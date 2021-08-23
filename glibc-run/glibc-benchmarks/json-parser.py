#! /usr/bin/env python3

import json
import sys
import statistics
import os

version_dirs = []

for version in sys.argv[1].split(","):
    if "STAR" in version:
        last_piece = version.split("/")[len(version.split("/")) - 1]
        dir_path = version.replace(last_piece, "")
        last_piece = last_piece.replace("STAR", "")
        for d in os.listdir(dir_path):
            if last_piece in d:
                version_dirs.append(dir_path + d)
    else:
        version_dirs.append(version)
version_dirs.sort()
print(str(version_dirs))
sys.exit(0)
benchmarks = []
for benchmark in sys.argv[2].split(","):
    if benchmark != "":
        benchmarks.append(benchmark)

cmp_impls = [sys.argv[3].split(",")]

score_cmp = None
if len(sys.argv) > 4:
    score_cmp = sys.argv[4]

bench_fmt = "/bench-{}.out"
cmp_files = []
for benchmark in benchmarks:
    cmp_files.append(bench_fmt.format(benchmark + "{}"))


def get_key(length, align1, align2, dgs, wfs, sz):
    return str(length) + "-" + str(align1) + "-" + str(align2) + "-" + str(
        dgs) + "-" + str(wfs) + "-" + str(sz)


def get_stat(times):
    #return statistics.geometric_mean(times)
    return statistics.median(times)


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


class Displayable():
    def __init__(self, hdr, times, cmp_idx):
        self.hdr = hdr
        self.times = times
        self.cmp_idx = cmp_idx

    def out(self):
        if len(self.times) > 2:
            if self.cmp_idx is None:
                return self.out_list()
            else:
                return self.out_list_score()
        return self.out_cmp()

    def out_list_score(self):
        out = self.hdr
        out = csv_add(out, str(round(self.times[self.cmp_idx], 3)))
        for i in range(0, len(self.times)):
            if i == self.cmp_idx:
                continue
            score = round(self.times[i] / self.times[self.cmp_idx], 3)
            out = csv_add(out, str(score))
        return out + "\n"

    def out_list(self):
        out = self.hdr
        for time in self.times:
            out = csv_add(out, str(round(time, 3)))
        return out + "\n"

    def out_cmp(self):
        out = csv_add(self.hdr, str(round(self.times[0], 3)))
        if self.times[1] is not None:
            score = round(self.times[0] / self.times[1], 3)
            out = csv_add(out, str(round(self.times[1], 3)))
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
        if ifunc not in self.timings and "fsrm" in ifunc:
            return get_stat(self.timings[ifunc.replace("fsrm", "erms")])
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

    def show_results(self, impls, others, cmp_s):
        if len(others) == 0:
            self.show_results_cmp_impls(impls)
        elif len(others) == 1:
            self.show_results_cmp_other(impls, others[0])
        else:
            if cmp_s is None or cmp_s == "":
                self.show_results_cmp_others(impls, others)
            else:
                self.show_result_scores(impls, others, cmp_s)

    def show_results_cmp_impls(self, impls):
        disps = []

        for key in self.key_order:
            times = [None, None]
            hdr = self.all_results[key].get_hdr()
            for i in range(0, len(impls)):
                times[i] = self.all_results[key].get_stat(
                    fmt_ifunc(self.bench_func, impls[i]))
            disps.append(Displayable(hdr, times, None))
        for disp in disps:
            print(disp.out(), end="")

    def show_results_cmp_other(self, impl, other):
        disps = []
        for key in self.key_order:
            times = [None, None]
            hdr = self.all_results[key].get_hdr()
            assert key in other.all_results
            assert other.all_results[key].get_hdr() == hdr
            assert other.bench_func == self.bench_func

            times[0] = self.all_results[key].get_stat(
                fmt_ifunc(self.bench_func, impl))

            times[1] = other.all_results[key].get_stat(
                fmt_ifunc(other.bench_func, impl))

            disps.append(Displayable(hdr, times, None))
        for disp in disps:
            print(disp.out(), end="")

    def show_result_scores(self, impl, others, cmp_s):
        disps = []

        for key in self.key_order:
            times = [
                self.all_results[key].get_stat(fmt_ifunc(
                    self.bench_func, impl))
            ]
            hdr = self.all_results[key].get_hdr()
            cmp_idx = None
            for i in range(0, len(others)):
                other = others[i]
                if cmp_s in other.file_fmt:
                    assert cmp_idx is None
                    cmp_idx = i + 1
#                print("{} -> {} [{}]".format(key, other.file_fmt, key in other.all_results))
                assert key in other.all_results
                assert other.all_results[key].get_hdr() == hdr
                assert other.bench_func == self.bench_func

                times.append(other.all_results[key].get_stat(
                    fmt_ifunc(self.bench_func, impl)))
            disps.append(Displayable(hdr, times, cmp_idx))
        for disp in disps:
            print(disp.out(), end="")

    def show_results_cmp_others(self, impl, others):
        disps = []
        for key in self.key_order:
            times = [
                self.all_results[key].get_stat(fmt_ifunc(
                    self.bench_func, impl))
            ]

            hdr = self.all_results[key].get_hdr()
            for other in others:
                assert key in other.all_results
                assert other.all_results[key].get_hdr() == hdr
                assert other.bench_func == self.bench_func

                times.append(other.all_results[key].get_stat(
                    fmt_ifunc(self.bench_func, impl)))

            disps.append(Displayable(hdr, times, None))
        for disp in disps:
            print(disp.out(), end="")


all_json_files = []
for i in range(0, len(version_dirs)):
    all_json_files.append([])
    for cmp_file in cmp_files:

        res = JsonFile(version_dirs[i] + cmp_file)
        res.parse_all_files()
        all_json_files[i].append(res)

if len(version_dirs) == 1:
    for res in all_json_files[0]:
        res.show_results_cmp_impls(cmp_impls)
else:
    for cmp_impl in cmp_impls:
        for i in range(0, len(all_json_files[0])):
            jsons_to_cmp = []
            for json_lists in all_json_files[1:]:
                jsons_to_cmp.append(json_lists[i])

            all_json_files[0][i].show_results(cmp_impl, jsons_to_cmp,
                                              score_cmp)
