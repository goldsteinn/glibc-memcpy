#! /usr/bin/env python3

import os
import sys

project_path = "/home/noah/programs/projects/memcpy"
partial_file = "partial-results.txt"
outfile = "res.txt"
run_func_icl = "memcpy_dev_v32_movsb"
run_func = run_func_icl

run_cmd = "./driver -v --core 0 --rt 100000 --func {} --hr --min 257 > {}".format(
    run_func, outfile)

nops = [0, 16, 32, 48]


def err(s):
    print(s)
    sys.exit(-1)


def make_key(ae, td, ev):
    return str(ae) + "-" + str(td) + "-" + str(ev)


class Result:
    def __init__(self, conf, time):
        self.conf = conf
        self.time = time

    def get_key(self):
        return self.conf.get_key()

    def csv_out(self):
        return "{},{}".format(self.conf.to_str(), self.time)


class Config:
    def __init__(self, p0, p1, p2):
        self.p0 = str(p0)
        self.p1 = str(p1)
        self.p2 = str(p2)

    def get_content(self):
        content = [
            "#ifndef _PADDING_CONF_H_", "#define  _PADDING_CONF_H_",
            "#define PADDING0 [0]", "#define PADDING1 [1]",
            "#define PADDING2 [2]", "#endif"
        ]
        out = ""
        for c in content:
            out += c.replace("[0]", "NOP" + self.p0).replace(
                "[1]", "NOP" + self.p1).replace("[2]", "NOP" + self.p2) + "\n"

        return out

    def get_key(self):
        return make_key(self.p0, self.p1, self.p2)

    def to_str(self):
        return "{},{},{}".format(self.p0, self.p1, self.p2)

    def write_config(self):
        path = project_path + "/src/impl/core/padding-conf.h"
        try:
            f = open(path, "w+")
            f.write(self.get_content())
            f.flush()
            f.close()
        except IOError:
            err("Error writing to: {}".format(path))


class Run:
    def __init__(self, conf):
        self.conf = conf
        self.time = None
        self.has_run = False

    def run(self):
        self.conf.write_config()
        ret = os.system(
            "(cd {}/build; make clean > /dev/null 2>&1; make > /dev/null 2>&1)"
            .format(project_path))
        assert ret == 0
        ret = os.system("(cd {}/build; {})".format(project_path, run_cmd))
        assert ret == 0
        self.has_run = True

        self.parse_output()

    def parse_output(self):
        assert self.has_run
        lines = []
        for line in open("{}/build/{}".format(project_path, outfile)):
            lines.append(line)

        time_line = None
        for line in lines:
            if "rand SPEC2017" in line:
                time_line = line

        assert time_line is not None

        scratch = time_line.split(",")
        self.time = float(scratch[len(scratch) - 1].lstrip().rstrip())

    def get_result(self):
        assert self.has_run
        return Result(self.conf, self.time)


results = {}

confs = []
for i in range(0, 4):
    for j in range(0, 4):
        for k in range(0, 4):
            confs.append(Config(nops[i], nops[j], nops[k]))

partialf = open(partial_file, "w+")
for conf in confs:
    r = Run(conf)
    r.run()
    res = r.get_result()
    partialf.write(res.csv_out() + "\n")
    partialf.flush()
    if res.get_key() in results:
        err("{} already in results".format(res.get_key()))
    results[res.get_key()] = res

partialf.close()
res_file = "results.txt"
f = open(res_file, "w+")
for conf in confs:
    res = results[conf.get_key()]
    f.write(res.csv_out() + "\n")
f.flush()
f.close()
