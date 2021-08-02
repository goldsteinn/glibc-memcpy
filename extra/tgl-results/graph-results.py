#! /usr/bin/env python3

import os
import matplotlib.pyplot as plt
import numpy as np
import csv
import argparse
import statistics

parser = argparse.ArgumentParser(
    description="Graph Performance of Memcpy Random Benchmark")
parser.add_argument("-f", action="store", default="", help="CSV File")
parser.add_argument(
    "-a",
    action="store_true",
    default=False,
    help="Graph one event or median cycles for varying alignments")
parser.add_argument("-e",
                    action="store_true",
                    default=False,
                    help="Graph multiple events with one alignment")
parser.add_argument("--events",
                    action="store",
                    default="",
                    help="CSV of events to track (empty for median cycles)")
parser.add_argument(
    "--alignments",
    action="store",
    default="16, 32, 48, 64",
    help=
    "CSV of alignments to track (empty for median cycles). If \"-e\" flag present uses first value"
)
parser.add_argument("--pmin",
                    action="store",
                    default="-1",
                    help="Min padding value (-1 to ignore)")
parser.add_argument("--pmax",
                    action="store",
                    default="-1",
                    help="Max padding value (-1 to ignore)")
parser.add_argument(
    "--pmod64",
    action="store",
    default="-1",
    help="Padding value == pmod64 (-1 to ignore, ignores lowest bit)")
parser.add_argument("--pmod2",
                    action="store",
                    default="0",
                    help="Padding value == pmod2 (-1 to ignore)")


class Event_Result:
    def __init__(self, ev_name, ev_count, ref_cycles):
        self.ev_name = ev_name
        self.ev_count = int(ev_count)
        self.ref_cycles = int(float(ref_cycles))

    def get_key(self):
        return self.ev_name


def alignment_result_key(al_entry, padding):
    return str(al_entry) + "-" + str(padding)


def make_event_title(s):
    if isinstance(s, int):
        return str(s)
    elif s == "":
        return "Median Ref Cycles"
    else:
        return "Event Count: {}".format(s)


class Alignment_Result:
    def __init__(self, al_entry, padding):
        self.al_entry = int(al_entry)
        self.padding = int(padding.replace("NOP", ""))

        self.ev_results = {}

    def get_key(self):
        return alignment_result_key(self.al_entry, self.padding)

    def get_ev_result(self, ev_name):
        assert ev_name in self.ev_results, "Event does not exist: {}".format(
            ev_name)
        return self.ev_results[ev_name]

    def get_ev_count(self, ev_name):
        return float(self.get_ev_result(ev_name).ev_count)

    def get_ev_ref_cycles(self, ev_name):
        return float(self.get_ev_result(ev_name).ref_cycles)

    def get_result_value(self, ev_name):
        if ev_name == "":
            return self.get_median_ref_cycles()
        else:
            return self.get_ev_count(ev_name)

    def aggregate_ref_cycles(self):
        all_ref_cycles = []
        for key in self.ev_results:
            all_ref_cycles.append(self.get_ev_ref_cycles(key))
        return all_ref_cycles

    def get_median_ref_cycles(self):
        return float(statistics.median(self.aggregate_ref_cycles()))

    def append_ev_result(self, ev_result):
        assert ev_result.get_key(
        ) not in self.ev_results, "Duplicate event: {}".format(
            ev_result.get_key())
        self.ev_results[ev_result.get_key()] = ev_result


class Padding_Constraint:
    def __init__(self, min_padding, max_padding, mod_64, mod_2):
        self.min_padding = min_padding
        self.max_padding = max_padding
        self.mod_64 = mod_64
        self.mod_2 = mod_2

    def valid(self, padding):
        if self.min_padding != -1 and self.min_padding > padding:
            return False
        if self.max_padding != -1 and self.max_padding < padding:
            return False
        if self.mod_64 != -1 and self.mod_64 != (
            (int(padding % 64) >> 1) << 1):
            return False
        if self.mod_2 != -1 and self.mod_2 != int(padding % 2):
            return False
        return True


class Grapher:
    def __init__(self, al_entrys, padding_constraints, ev_names):
        self.al_entrys = al_entrys
        self.padding_constraints = padding_constraints
        self.ev_names = ev_names

        self.colors = ['r', 'g', 'b', 'y', 'm']

    def include_al_entry(self, al_entry):
        return al_entry in self.al_entrys

    def include_padding(self, padding):
        return self.padding_constraints.valid(padding)

    def include_ev_name(self, ev_name):
        return (self.ev_names[0] == ""
                and len(self.ev_names) == 1) or ev_name in self.ev_names

    def filter_fields(self, al_results):
        results_to_graph = []
        al_entry_map = {}
        padding_map = {}

        for result_key in al_results:
            result = al_results[result_key]
            if self.include_padding(result.padding) and self.include_al_entry(
                    result.al_entry):
                results_to_graph.append(result)
                if result.al_entry not in al_entry_map:
                    al_entry_map[result.al_entry] = -1
                if result.padding not in padding_map:
                    padding_map[result.padding] = -1

        assert len(padding_map) > 0
        assert len(al_entry_map) > 0
        assert len(results_to_graph) > 0
        return results_to_graph, al_entry_map, padding_map

    def order_fields(self, field_map):
        field_indexes = []
        for field in field_map:
            field_indexes.append(field)
        field_indexes.sort()

        for i in range(0, len(field_indexes)):
            field_map[field_indexes[i]] = i
        return field_map, field_indexes

    def prepare_result_values(self, i_indexes, j_indexes):
        result_values = []
        for i in range(0, len(i_indexes)):
            result_values.append([0.0] * len(j_indexes))
        return result_values

    def verify_result_values(self, result_values):
        for i_data in result_values:
            for j_data in i_data:
                assert j_data != 0.0, "Missing data!"

    def find_seperator(self, result_values):
        max_vals = []
        for i_data in result_values:
            max_vals.append(max(i_data))

        max_ratio = 0.0
        max_ratio_idx = 0
        for i in range(0, len(max_vals) - 1):
            lo = min(max_vals[i], max_vals[i + 1])
            hi = max(max_vals[i], max_vals[i + 1])
            if max_ratio < (hi / lo):
                max_ratio_idx = i
                max_ratio = hi / lo
        if max_ratio > 100.0:
            return max_ratio_idx + 1
        return len(result_values)

    def plot_results_2y(self, i_indexes, j_indexes, i_seperator, ylabel, title,
                        result_values):
        fig, ax = plt.subplots()
        x_axis = np.arange(len(j_indexes))
        bar_width = 1.0 / float(len(i_indexes) + 1)
        for i in range(0, i_seperator):
            ax.bar(x_axis + bar_width * float(i),
                   result_values[i],
                   label=make_event_title(i_indexes[i]),
                   color=self.colors[i],
                   width=bar_width)

        plt.xticks(x_axis, j_indexes)
        ax.set_ylabel(ylabel)
        ax.set_xlabel("Padding Bytes")
        ax2 = ax.twinx()
        for i in range(i_seperator, len(i_indexes)):
            ax2.bar(x_axis + bar_width * float(i),
                    result_values[i],
                    label=str(i_indexes[i]),
                    color=self.colors[i],
                    width=bar_width)
        ax2.set_ylabel(ylabel)
        ax.legend(loc='upper left')
        ax2.legend(loc='upper right')
        plt.title(title)
        plt.show()

    def plot_results(self, i_indexes, j_indexes, ylabel, title, result_values):
        x_axis = np.arange(len(j_indexes))
        bar_width = 1.0 / float(len(i_indexes) + 1)
        for i in range(0, len(i_indexes)):
            plt.bar(x_axis + bar_width * float(i),
                    result_values[i],
                    label=make_event_title(i_indexes[i]),
                    color=self.colors[i],
                    width=bar_width)
        plt.xticks(x_axis, j_indexes)
        plt.ylabel(ylabel)
        plt.xlabel("Padding Bytes")
        plt.title(title)
        plt.legend()
        plt.show()

    def plot_ev_names(self, al_results):
        results_to_graph, al_entry_map, padding_map = self.filter_fields(
            al_results)
        padding_map, padding_indexes = self.order_fields(padding_map)
        result_values = self.prepare_result_values(self.ev_names,
                                                   padding_indexes)

        for result in results_to_graph:
            for i in range(0, len(self.ev_names)):
                j = padding_map[result.padding]
                result_values[i][j] = result.get_result_value(self.ev_names[i])

        self.verify_result_values(result_values)

        i_seperator = self.find_seperator(result_values)
        self.plot_results_2y(
            self.ev_names, padding_indexes, i_seperator, "Counter Value",
            "Memcpy Perf Counters w/ Varying Padding, Align Entry = {}".format(
                self.al_entrys[0]), result_values)

    def plot_al_entrys(self, al_results):
        results_to_graph, al_entry_map, padding_map = self.filter_fields(
            al_results)

        padding_map, padding_indexes = self.order_fields(padding_map)
        al_entry_map, al_entry_indexes = self.order_fields(al_entry_map)

        result_values = self.prepare_result_values(al_entry_indexes,
                                                   padding_indexes)

        for result in results_to_graph:
            i = al_entry_map[result.al_entry]
            j = padding_map[result.padding]
            result_values[i][j] = result.get_result_value(self.ev_names[0])

        self.verify_result_values(result_values)

        self.plot_results(
            al_entry_indexes, padding_indexes,
            make_event_title(self.ev_names[0]),
            "Memcpy Performance w/ Varying Padding / Entry Alignment",
            result_values)


def parse_csv(fname):
    all_al_results = {}
    with open(fname) as csvfile:
        header = [h.strip() for h in csvfile.readline().split(',')]
        reader = csv.DictReader(csvfile, fieldnames=header)
        for line in reader:
            al_entry = line["align_entry"]
            padding = line["padding"]
            ev_name = line["ev_name"]
            ev_count = line["ev_count"]
            ref_cycles = line["ref_cycles"]
            al_result = Alignment_Result(al_entry, padding)
            ev_result = Event_Result(ev_name, ev_count, ref_cycles)
            if al_result.get_key() not in all_al_results:
                all_al_results[al_result.get_key()] = al_result

            all_al_results[al_result.get_key()].append_ev_result(ev_result)
        return all_al_results


args = parser.parse_args()
fname = args.f
assert fname != "" and os.path.isfile(fname)

graph_alignments = args.a
graph_events = args.e
assert graph_events or graph_alignments
assert graph_events is not graph_alignments

events_dups = args.events.split(",")
events_set = {}
for event in events_dups:
    events_set[event] = True
events = []
for event in events_set:
    events.append(event)

al_entry_strs = args.alignments.split(",")
assert len(al_entry_strs) > 0
al_entrys = []
for al_entry_str in al_entry_strs:
    al_entrys.append(int(al_entry_str))
    if graph_events:
        break

pmin = int(args.pmin)
pmax = int(args.pmax)
pmod64 = int(args.pmod64)
pmod2 = int(args.pmod2)

padding_contraints = Padding_Constraint(pmin, pmax, pmod64, pmod2)

grapher = Grapher(al_entrys, padding_contraints, events)
if graph_events:
    grapher.plot_ev_names(parse_csv(fname))
else:
    grapher.plot_al_entrys(parse_csv(fname))
