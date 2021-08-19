#! /usr/bin/env python3

import json
import sys
import statistics
import os

impl_dir = sys.argv[1]
benchmarks = []
for benchmark in sys.argv[2].split(","):
    if benchmark != "":
        benchmarks.append(benchmark)

cmp_impls = [sys.argv[3].split(",")]
if len(sys.argv) > 4:
    cmp_impls.append(sys.argv[4].split(","))

bench_fmt = "{}/bench-{}.out"
cmp_files = []
for benchmark in benchmarks:
    cmp_files.append(bench_fmt.format(impl_dir, benchmark + "{}"))


def get_key(length, align1, align2, dgs, wfs, sz):
    return str(length) + "-" + str(align1) + "-" + str(align2) + "-" + str(
        dgs) + "-" + str(wfs) + "-" + str(sz)


def get_stat(times):
    return statistics.geometric_mean(times)


def csv_add(s, field):
    if field is None:
        return s
    if s != "":
        s += ","
    return s + str(field)


def fmt_ifunc(bench_func, impl):
    out = "__{}_{}_unaligned".format(bench_func, impl[0])
    for i in range(1, len(impl)):
        out += "_" + impl[i]
    return out


def set_if_exists(json_map, field, field_val):
    if field in json_map:
        return str(json_map[field]).lstrip().rstrip()
    return field_val


def dgs_to_str(dgs):
    if dgs is None:
        return dgs

    if dgs == "0":
        return "Forward"
    if dgs == "1":
        return "Backward"
    if dgs == "-1":
        return "Bidirectional"
    return dgs



total = 0.0
class Displayable():
    def __init__(self, hdr, t0, t1):
        self.hdr = hdr
        self.t0 = t0
        self.t1 = t1

    def out(self):
        out = csv_add(self.hdr, str(round(self.t0, 3)))
        if self.t1 is not None:
            score = round(self.t0 / self.t1, 3)
            global total
            total += score
            out = csv_add(out, str(round(self.t1, 3)))
            while out.count(',') < 5:
                out += ","
            out = csv_add(out, str(score))
        return out + "\n"


class Result():
    def __init__(self, ifuncs, length, align1, align2, dgs, wfs, sz):
        self.ifuncs = ifuncs
        self.length = length
        self.align1 = align1
        self.align2 = align2
        self.dgs = dgs
        self.wfs = wfs
        self.sz = sz
        self.timings = {}
        for ifunc in ifuncs:
            self.timings[ifunc] = []

    def add_times(self, times):
        assert len(times) == len(self.ifuncs)
        for i in range(0, len(self.ifuncs)):
#            if int(self.length) == 8192 and int(self.align1) == 0 and int(self.align2) == 0:
#                print("{} -> {}".format(self.ifuncs[i], times[i]))
            self.timings[self.ifuncs[i]].append(float(times[i]))

    def get_stat(self, ifunc):
        return get_stat(self.timings[ifunc])

    def get_hdr(self):
        out = csv_add("", self.length)
        out = csv_add(out, self.align1)
        out = csv_add(out, self.align2)
        out = csv_add(out, dgs_to_str(self.dgs))
        out = csv_add(out, self.wfs)
        out = csv_add(out, self.sz)
        return out

    def result_key(self):
        return get_key(self.length, self.align1, self.align2, self.dgs,
                       self.wfs, self.sz)


class JsonFile():
    def __init__(self, file_fmt):
        self.file_fmt = file_fmt
        self.key_order = []
        self.all_results = {}
        self.bench_func = ""

    def load_file(self, fname):
        if os.access(fname, os.R_OK) is False:
            return None
        with open(fname) as json_file:
            return json.load(json_file)

    def get_results(self, json_obj):
        for k in json_obj["functions"]:
            if self.bench_func == "":
                self.bench_func = k
            assert self.bench_func == k
        results = json_obj["functions"][self.bench_func]["results"]
        ifuncs = json_obj["functions"][self.bench_func]["ifuncs"]

        for result in results:
            length = None
            align1 = None
            align2 = None
            dgs = None
            wfs = None
            sz = None
            length = set_if_exists(result, "length", length)
            length = set_if_exists(result, "max-alignment", length)
            align1 = set_if_exists(result, "align1", align1)
            align2 = set_if_exists(result, "align2", align2)
            dgs = set_if_exists(result, "dst > src", dgs)
            wfs = set_if_exists(result, "with-fixed-size", wfs)
            sz = set_if_exists(result, "size", sz)
            key = get_key(length, align1, align2, dgs, wfs, sz)

            if key not in self.all_results:
                self.key_order.append(key)
                self.all_results[key] = Result(ifuncs, length, align1, align2,
                                               dgs, wfs, sz)
            self.all_results[key].add_times(result["timings"])

    def parse_all_files(self):
        for i in range(0, 100):
            file_path = self.file_fmt.format(i)

            json_obj = self.load_file(file_path)
            if json_obj is None:
                continue

            self.get_results(json_obj)

    def show_results(self, impls):
        disps = []
        for key in self.key_order:
            times = [None, None]
            hdr = self.all_results[key].get_hdr()
#            if int(self.all_results[key].length) <= 64:
#                continue
            for i in range(0, len(impls)):
                times[i] = self.all_results[key].get_stat(
                    fmt_ifunc(self.bench_func, impls[i]))
            disps.append(Displayable(hdr, times[0], times[1]))
        for disp in disps:
            print(disp.out(), end="")



for cmp_file in cmp_files:
    res = JsonFile(cmp_file)
    res.parse_all_files()
    res.show_results(cmp_impls)
print("{}".format(total))
