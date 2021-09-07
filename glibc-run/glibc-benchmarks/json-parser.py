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
        version_dirs.sort()
    else:
        version_dirs.append(version)

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

time_offset = 4


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


def set_if_exists(json_map, field, field_val, fields):
    if field in json_map:
        fields[field] = True
        return str(json_map[field]).lstrip().rstrip(), fields

    return field_val, fields


def dgs_to_str(dgs):
    if dgs is None:
        return dgs
    return dgs
    if dgs == "0":
        return "Forward"
    if dgs == "1":
        return "Backward"
    if dgs == "-1":
        return "Bidirectional"
    return dgs


def add_offset(out):
    global time_offset
    while out.count(',') < time_offset:
        out += ","
    return out


class Displayable():
    def __init__(self, hdr, times, cmp_idx):
        self.hdr = hdr
        self.times = times
        self.cmp_idx = cmp_idx

    def out(self):
        if len(self.times) > 2:
            if self.cmp_idx is None:
                return self.out_list()
            elif self.cmp_idx == -1:
                return self.out_cmp_many()
            else:
                return self.out_list_score()
        return self.out_cmp()

    def out_list_score(self):
        out = self.hdr
        out = add_offset(out)
        out = csv_add(out, str(round(self.times[self.cmp_idx], 3)))
        for i in range(0, len(self.times)):
            if i == self.cmp_idx:
                continue
            score = round(self.times[i] / self.times[self.cmp_idx], 3)
            out = csv_add(out, str(score))
        return out + "\n"

    def out_list(self):
        out = self.hdr
        out = add_offset(out)
        for time in self.times:
            out = csv_add(out, str(round(time, 3)))
        return out + "\n"

    def get_cmp_score(self, t0, t1):
        return str(round(t0, 3)), str(round(t1, 3)), str(round(t0 / t1, 3))

    def out_cmp_many(self):
        out = self.hdr
        out = add_offset(out)
        for i in range(0, int(len(self.times) / 2)):
            t0, t1, score = self.get_cmp_score(self.times[2 * i],
                                               self.times[2 * i + 1])
            out = csv_add(out, t0)
            out = csv_add(out, t1)
            out = csv_add(out, score)
            if i != int(len(self.times) / 2) - 1:
                out += ","
        return out + "\n"

    def out_cmp(self):
        out = self.hdr
        out = add_offset(out)
        out = csv_add(self.hdr, str(round(self.times[0], 3)))
        if self.times[1] is not None:
            score = round(self.times[0] / self.times[1], 3)
            out = csv_add(out, str(round(self.times[1], 3)))
            out = csv_add(out, str(score))
        return out + "\n"


class Result():
    def __init__(self, ifuncs, fields, length, align1, align2, dgs, wfs, sz):
        self.ifuncs = ifuncs
        self.fields = fields
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
        if self.wfs is not None:
            out = csv_add(out, str(self.sz))
        else:
            out = csv_add(out, self.sz)
        return out

    def result_key(self):
        return get_key(self.length, self.align1, self.align2, self.dgs,
                       self.wfs, self.sz)


class JsonFile():
    def __init__(self, file_fmt, replacement):
        self.must_replace = False
        self.replacement = replacement
        self.file_fmt = file_fmt
        self.key_order = []
        self.all_results = {}
        self.bench_func = ""
        self.fields = {}

    def name(self, impl):
        ret = ""
        if "glibc" not in self.file_fmt:
            ret = "New"
        if "glibc" in self.file_fmt:
            ret = "Cur"
        return ret + "-" + impl

    def out_fields(self):
        out = ""
        for field in self.fields:
            out = csv_add(out, field)
        return out

    def func_hdr(self):

        bench = self.file_fmt[self.file_fmt.find("bench-") +
                              len("bench-"):self.file_fmt.find("{}")]
        return "\nResults For: {}".format(bench)

    def csv_hdr(self, other, impls):
        out = self.out_fields()
        out = add_offset(out)
        for i in range(0, len(impls)):
            out = csv_add(out, self.name(impls[i]))
            if other is not None:
                out = csv_add(out, other.name(impls[i]))
                out = csv_add(out, "Score-{}".format(impls[i]))
            if i != len(impls) - 1:
                out += ","

        return out

    def fmt_ifunc(self, impl):
        impl_pieces = impl.split("-")
        postfix = "_unaligned"
        if self.get_bench_func() == "memcmp" or self.get_bench_func(
        ) == "bcmp":
            if self.get_bench_func() == "memcmp" and (impl == "avx2"
                                                          or impl == "evex"):
                postfix = "_movbe"
            else:
                postfix = ""
        out = "__{}_{}{}".format(self.get_bench_func(), impl_pieces[0],
                                     postfix)
        if len(impl_pieces) > 1:
            out += "_" + impl_pieces[1]
        return out

    def load_file(self, fname):
        if os.access(fname, os.R_OK) is False:
            return None
        with open(fname) as json_file:
            return json.load(json_file)

    def get_bench_func(self):
        return self.bench_func

    def eq_bench_func(self, other):
        scmp = self.bench_func
        ocmp = other.bench_func
        if self.must_replace is True:
            scmp = self.replacement[0]
        if other.must_replace is True:
            ocmp = other.replacement[0]
        return ocmp == scmp

    def set_bench_func(self, k):
        if self.bench_func == "":
            self.bench_func = k
        assert self.bench_func == k

    def get_results(self, json_obj):
        for k in json_obj["functions"]:
            self.set_bench_func(k)
        results = json_obj["functions"][self.get_bench_func()]["results"]
        ifuncs = json_obj["functions"][self.get_bench_func()]["ifuncs"]

        for result in results:
            length = None
            align1 = None
            align2 = None
            dgs = None
            wfs = None
            sz = None
            length, self.fields = set_if_exists(result, "length", length,
                                                self.fields)
            length, self.fields = set_if_exists(result, "max-alignment",
                                                length, self.fields)
            length, self.fields = set_if_exists(result, "region-size", length,
                                                self.fields)
            align1, self.fields = set_if_exists(result, "align1", align1,
                                                self.fields)
            align2, self.fields = set_if_exists(result, "align2", align2,
                                                self.fields)
            dgs, self.fields = set_if_exists(result, "dst > src", dgs,
                                             self.fields)
            wfs, self.fields = set_if_exists(result, "with-fixed-size", wfs,
                                             self.fields)
            sz, self.fields = set_if_exists(result, "overlap", sz, self.fields)
            sz, self.fields = set_if_exists(result, "size", sz, self.fields)
            sz, self.fields = set_if_exists(result, "result", sz, self.fields)
            
            key = get_key(length, align1, align2, dgs, wfs, sz)
            if "memmove" in self.get_bench_func() and align1 == align2:
                continue
            if key not in self.all_results:
                self.key_order.append(key)
                self.all_results[key] = Result(ifuncs, self.fields, length,
                                               align1, align2, dgs, wfs, sz)
            self.all_results[key].add_times(result["timings"])

    def parse_all_files(self):
        for i in range(0, 100):
            file_path = self.file_fmt.format(i)

            json_obj = self.load_file(file_path)
            if json_obj is None:
                if self.must_replace:
                    continue
                json_obj = self.load_file(
                    file_path.replace(self.replacement[0],
                                      self.replacement[1]))
                if json_obj is None:
                    continue
                else:
                    self.must_replace = True
                    self.file_fmt = self.file_fmt.replace(
                        self.replacement[0], self.replacement[1])

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
        print(self.func_hdr())
        for key in self.key_order:
            times = [None, None]
            hdr = self.all_results[key].get_hdr()
            for i in range(0, len(impls)):
                times[i] = self.all_results[key].get_stat(
                    fmt_ifunc(self.get_bench_func(), impls[i]))
            disps.append(Displayable(hdr, times, None))
        for disp in disps:
            print(disp.out(), end="")

    def show_results_cmp_other(self, impls, other):

        disps = []
        print(self.func_hdr())
        print(self.csv_hdr(other, impls))
        for key in self.key_order:
            times = []
            hdr = self.all_results[key].get_hdr()

            assert key in other.all_results
            assert other.all_results[key].get_hdr() == hdr
            assert self.eq_bench_func(other)
            for impl in impls:
                times.append(self.all_results[key].get_stat(
                    self.fmt_ifunc(impl)))
                times.append(other.all_results[key].get_stat(
                    other.fmt_ifunc(impl)))

            disps.append(Displayable(hdr, times, -1))
        for disp in disps:
            print(disp.out(), end="")

    def show_result_scores(self, impl, others, cmp_s):
        disps = []
        print(self.func_hdr())
        print(self.out_fields())
        impl = impl[0]
        for key in self.key_order:
            times = [
                self.all_results[key].get_stat(
                    fmt_ifunc(self.get_bench_func(), impl))
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
                assert other.get_bench_func() == self.get_bench_func()

                times.append(other.all_results[key].get_stat(
                    fmt_ifunc(self.get_bench_func(), impl)))
            disps.append(Displayable(hdr, times, cmp_idx))
        for disp in disps:
            print(disp.out(), end="")

    def show_results_cmp_others(self, impl, others):
        disps = []
        print(self.func_hdr())
        print(self.out_fields())
        for key in self.key_order:
            times = [
                self.all_results[key].get_stat(
                    fmt_ifunc(self.get_bench_func(), impl))
            ]

            hdr = self.all_results[key].get_hdr()
            for other in others:
                assert key in other.all_results
                assert other.all_results[key].get_hdr() == hdr
                assert other.get_bench_func() == self.get_bench_func()

                times.append(other.all_results[key].get_stat(
                    fmt_ifunc(self.get_bench_func(), impl)))

            disps.append(Displayable(hdr, times, None))
        for disp in disps:
            print(disp.out(), end="")

all_json_files = []
for i in range(0, len(version_dirs)):
    all_json_files.append([])
    for cmp_file in cmp_files:

        res = JsonFile(version_dirs[i] + cmp_file, ["bcmp", "memcmp"])
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
