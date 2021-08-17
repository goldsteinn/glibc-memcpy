#! /usr/bin/env python3

import json
import sys
import statistics as stats
import os

assert len(sys.argv) > 1
impl_dir = sys.argv[1]
do_norm = sys.argv[2] == "N"
do_func = sys.argv[3]

base_includes = ["avx2", "evex", "", "avx512", "erms"]
todo_files = [
    "bench-{}{}.out", "bench-{}-large{}.out", "bench-{}-walk{}.out",
    "bench-{}-random{}.out"
]
cmp_files = ["bench-{}{}.out"]

includes = []
raw_files = [
    "memset-walk{}", "memset-large{}", "memset{}", "mempcpy{}",
    "memcpy-walk{}", "memcpy-random{}", "memcpy-large{}", "memcpy{}",
    "memccpy{}", "memmove{}", "memmove-large{}", "memmove-walk{}"
]

files = []
for f in raw_files:
    files.append("bench-" + f + ".out")

impls = ["old"]
if len(sys.argv) > 4:
    impls.append(sys.argv[4])
else:
    impls.append("new")
if len(sys.argv) > 5:
    todos = sys.argv[5].split(",")
    for t in todos:
        for f in todo_files:
            if t in f:
                if f not in cmp_files:
                    cmp_files.append(f)

if len(sys.argv) > 6:
    for i in range(6, len(sys.argv)):
        includes.append(sys.argv[i])
else:
    includes = base_includes

impl_cpu = ""  # impl_dir.split("-")[0]
#for f in os.listdir(impl_dir):
#    f = impl_dir + "/" + f
#    f = f.replace("//", "/")
#    impls.append(f)

nruns = 200
do_csv = True


def get_key(length, align1, align2, dgs):
    return str(length) + "-" + str(align1) + "-" + str(align2) + "-" + str(dgs)


class Result():
    def __init__(self, ifuncs, length, align1, align2, dgs):
        self.ifuncs = ifuncs
        self.length = length
        self.align1 = align1
        self.align2 = align2
        self.dgs = dgs
        self.timings = []


#        print("{} -> {}, {}, {}, {}".format(self.result_key(), self.length, self.align1, self.align2, self.dgs))

    def add_times(self, times):
        assert len(times) == len(self.ifuncs)
        self.timings.append(times)

    def result_key(self):
        return get_key(self.length, self.align1, self.align2, self.dgs)

    def get_stats(self, times):
        mean = stats.mean(times)
        mean_1_3 = stats.mean(times[int(len(times) / 4):int((3 * len(times)) /
                                                            4)])
        median = stats.median(times)
        gmean = stats.geometric_mean(times)
        return round(mean, 3), round(mean_1_3, 3), round(median, 3), round(
            min(times), 3), round(max(times), 3), round(gmean, 3)

    def function_stats(self, func, norm_func):
        idx = self.ifuncs.index(func)

        norm_idx = -1
        if norm_func != "":
            norm_idx = self.ifuncs.index(norm_func)

        times = []
        norm_times = []

        for t in self.timings:
            times.append(t[idx])
            if norm_idx != -1:
                norm_times.append(t[idx] / t[norm_idx])
        times.sort()
        norm_times.sort()
        if norm_idx == -1:
            return self.get_stats(times)
        else:
            return self.get_stats(norm_times)

    def pretty_print(self, impl, func, norm_func):
        mean, mean_1_3, median, _min, _max, gmean = self.function_stats(
            func, norm_func)
        impl_name = "Old"
        if "new" in impl:
            impl_name = "New"
        print("[{0:<4}, {1:<4}, {2:<8}, {3:<4},  {4:<4}, {5:<2}] -> ".format(
            func.replace("__memset_", ""), impl_name, self.length, self.align1,
            self.align2, dgs) +
              "[{0:<10}, {1:<10}, {2:<10},{3:<10}, {4:<10}]".format(
                  gmean, mean_1_3, median, _min, _max))

    def csv_print(self, impl, func, norm_func, func_type):
        nmean, nmean_1_3, nmedian, n_min, n_max, ngmean = self.function_stats(
            func, norm_func)
        mean, mean_1_3, median, _min, _max, gmean = self.function_stats(
            func, "")

        impl_name = impl.split("/")[2].replace(func_type + "-", "")

        impl_name = "Old"
        if "new" in impl:
            impl_name = "New"

        ext_name = "evex"
        erm_name = "-erms"
        if "avx512" in func:
            ext_name = "avx512"
        if "avx" in func:
            ext_name = "avx2"
        if "sse" in func:
            ext_name = "sse2"
        if "erms" not in func:
            erm_name = ""

        if norm_func == "":
            print("{},{},{},{},{},{},{},{}".format(ext_name + erm_name,
                                                   impl_name, self.length,
                                                   self.align1, self.align2,
                                                   self.dgs, median, gmean))
        else:
            print("{},{},{},{},{},{},{},{},{},{}".format(
                ext_name + "-" + erm_name, impl_name, self.length, self.align1,
                self.align2, self.dgs, median, nmedian, gmean, ngmean))

    def pretty_key(self):
        return "{0:<8}, {1:<3}, {2:<3}, {3:<2}".format(self.length,
                                                       self.align1,
                                                       self.align2, self.dgs)


def kvp_cmp(a):
    return a[1]


def change_str(val_new, val_old):
    postfix = "New"
    if val_new == val_old:
        postfix = "Equal"
    if val_new > val_old:
        postfix = "Current"

    return "{0:<10} -> {1}".format(str(round(float(val_new / val_old), 1)),
                                   postfix)


class JsonFile():
    def __init__(self, file_fmt, cmp_func, func_type, norm_func):
        self.ifuncs = []
        self.func_type = func_type
        self.norm_func = norm_func
        self.ncmp = len(file_fmt)
        self.cmp_func = cmp_func
        self.file_fmt = file_fmt
        self.results = []
        for i in range(0, self.ncmp):
            self.results.append({})

    def get_file_results(self, json_obj, idx):
        json_results = json_obj["functions"][self.func_type]["results"]
        for r in json_results:
            length = "0"
            align1 = "4096"
            align2 = "4096"
            dgs = "0"
            if "length" in r:
                length = r["length"]
                if "alignment" in r:
                    align1 = r["alignment"]
                if "align1" in r:
                    align1 = r["align1"]
                if "align2" in r:
                    align2 = r["align2"]
                if "dst > src" in r:
                    dgs = r["dst > src"]
            else:
                length = r["max-size"]
                if "dst > src" in r:
                    dgs = r["dst > src"]
            key = get_key(length, align1, align2, dgs)
            if key not in self.results[idx]:
                self.results[idx][key] = Result(
                    json_obj["functions"][self.func_type]["ifuncs"], length,
                    align1, align2, dgs)
            assert key == self.results[idx][key].result_key()
            times = r["timings"]
            float_times = [float(t) for t in times]
            self.results[idx][key].add_times(float_times)

    def load_file(self, fname):
        if os.access(fname, os.R_OK) is False:
            return None
        with open(fname) as json_file:
            return json.load(json_file)

    def parse_all_files(self):
        for idx in range(0, self.ncmp):
            for i in range(0, nruns):
                json_obj = self.load_file(self.file_fmt[idx].format(
                    self.func_type, i))
                if json_obj is None:
                    continue

                if self.cmp_func not in json_obj["functions"][
                        self.func_type]["ifuncs"]:
                    continue
                self.get_file_results(json_obj, idx)

    def show_all_results(self, empty):
        extra = ""
        if do_csv is False:
            extra = "######################################################################\n"

        for key in self.results[0]:
            for i in range(1, self.ncmp):
                assert key in self.results[i]
            maxsize = 32
            if "avx512" in self.cmp_func:
                maxsize = 64
            if False and self.results[0][
                    key].length <= maxsize and "rand" not in self.file_fmt[0]:
                continue

            for i in range(0, self.ncmp):
                if do_csv is True:
                    self.results[i][key].csv_print(self.file_fmt[i],
                                                   self.cmp_func,
                                                   self.norm_func,
                                                   self.func_type)
                else:
                    self.results[i][key].pretty_print(self.file_fmt[i],
                                                      self.cmp_func,
                                                      self.norm_func)

            print(extra, end='')

    def show_cmp_results(self, hdr):
        if hdr is True:
            print(
                "{0:<20}, {1:<8}, {2:<3}, {3:<3}, {4:<2}, {5:<10}, {6:<10}, {7:<10}"
                .format("Function", "Length", "Al1", "Al2", "D", "New Time",
                        "Cur Time", "%% Change"))

        for key in self.results[0]:
            pairs = []
            for i in range(1, self.ncmp):
                assert key in self.results[i]
            for i in range(0, self.ncmp):
                mean, mean_1_3, median, _min, _max, gmean = self.results[i][
                    key].function_stats(self.cmp_func)
                pairs.append([
                    impls[i].replace(".S", ""), median,
                    self.results[i][key].pretty_key()
                ])

            print("{0:<20}, {1:<16}, {2:<10}, {3:<10}, {4:<10}".format(
                self.cmp_func.replace("__{}".format(self.func_type), ""),
                pairs[0][2], pairs[0][1], pairs[1][1],
                change_str(pairs[0][1], pairs[1][1])))


cmp_func_base = []

if "sse2" in includes:
    if "erms" in includes:
        cmp_func_base.append("sse2_unaligned_erms")
    else:
        cmp_func_base.append("sse2_unaligned")
    if "" in includes:
        cmp_func_base.append("sse2_unaligned")
if "avx2" in includes:
    if "erms" in includes:
        cmp_func_base.append("avx_unaligned_erms")
    else:
        cmp_func_base.append("avx_unaligned")
    if "" in includes:
        cmp_func_base.append("avx_unaligned")

if "evex" in includes:
    if "erms" in includes:
        cmp_func_base.append("evex_unaligned_erms")
    else:
        cmp_func_base.append("evex_unaligned")
    if "" in includes:
        cmp_func_base.append("evex_unaligned")
if "avx512" in includes:

    if "erms" in includes:
        cmp_func_base.append("avx512_unaligned_erms")
    else:
        cmp_func_base.append("avx512_unaligned")
    if "" in includes:
        cmp_func_base.append("avx512_unaligned")
cmp_functions = []
for cf in cmp_func_base:
    cmp_functions.append("__" + do_func + "_" + cf)
norm_functions = ""
if do_norm is True:
    if do_func == "memcpy":
        norm_functions = "__memcpy_avx_unaligned"
    else:
        norm_functions = "generic_" + do_func

#cmp_files = ["bench-{}{}.out"]

# ,"bench-{}-large{}.out","bench-{}-walk{}.out", "bench-{}-random{}.out"
jsons = []
for cmp_file in cmp_files:
    for cmp_func in cmp_functions:
        path_fmt = "{}/{}/{}/{}"
        paths = []
        for impl in impls:
            paths.append(path_fmt.format(impl_dir, impl, do_func, cmp_file))

        jsons.append(JsonFile(paths, cmp_func, do_func, norm_functions))

first = True
for jf in jsons:
    jf.parse_all_files()
    jf.show_all_results(False)
    first = False
