#! /usr/bin/env python3

import os
import sys

project_path = "/home/noah/programs/projects/memcpy"
partial_file = "partial-results.txt"
outfile = "res.txt"
run_cmd = "sudo ./driver -v --core 0 --rt 200000 --func memcpy_dev_v32_movsb --hr --max 32 > " + outfile

align_entry = [16, 32, 48, 64]
padding = []
i = 0
while i <= 1024:
    padding.append("NOP{}".format(i))
    padding.append("NOP{}".format(i + 1))
    i += 16

events = [
    "INT_MISC_CLEAR_RESTEER_CYCLES", "INT_MISC_RECOVERY_CYCLES",
    "INT_MISC_UOP_DROPPING", "UOPS_RETIRED_SLOTS", "UOPS_ISSUED_ANY",
    "UOPS_EXECUTED_CORE", "IDQ_UOPS_NOT_DELIVERED_CORE", "IDQ_MS_UOPS",
    "IDQ_MITE_UOPS", "IDQ_DSB_UOPS", "DSB2MITE_SWITCHES_PENALTY_CYCLES",
    "LSD_UOPS", "BR_INST_RETIRED_COND", "BRANCH_INSTRUCTIONS_RETIRED",
    "BR_INST_RETIRED_COND_TAKEN", "BR_INST_RETIRED_COND_NTAKEN",
    "MISPREDICTED_BRANCH_RETIRED", "BR_MISP_RETIRED_COND_TAKEN",
    "BR_MISP_RETIRED_COND_NTAKEN", "BACLEARS_ANY", "UOPS_DISPATCHED_PORT_0",
    "UOPS_DISPATCHED_PORT_1", "UOPS_DISPATCHED_PORT_2_3",
    "UOPS_DISPATCHED_PORT_4_9", "UOPS_DISPATCHED_PORT_5",
    "UOPS_DISPATCHED_PORT_6", "UOPS_DISPATCHED_PORT_7_8"
]


def err(s):
    print(s)
    sys.exit(-1)


def make_key(ae, td, ev):
    return str(ae) + "-" + str(td) + "-" + str(ev)


class Result:
    def __init__(self, conf, time, perf_count):
        self.conf = conf
        self.time = time
        self.perf_count = perf_count

    def get_key(self):
        return self.conf.get_key()

    def csv_out(self):
        return "{},{},{},{},{}".format(self.conf.ae, self.conf.td,
                                       self.conf.ev, self.time,
                                       self.perf_count)


class Config:
    def __init__(self, ae, td, ev):
        self.ae = str(ae)
        self.td = str(td)
        self.ev = str(ev)

    def get_content(self):
        content = [
            "#ifndef _CONFIG_SHIT_H_", "#define  _CONFIG_SHIT_H_",
            "#define ALIGN_ENTRY [ALIGN_ENTRY]", "#define PADDING [PADDING]",
            "#define EVENT_TO_CHECK [EVENT_TO_CHECK]", "#endif"
        ]
        out = ""
        for c in content:
            out += c.replace("[ALIGN_ENTRY]", self.ae).replace(
                "[PADDING]", self.td).replace("[EVENT_TO_CHECK]",
                                              self.ev) + "\n"

        return out

    def get_key(self):
        return make_key(self.ae, self.td, self.ev)

    def write_config(self):
        path = project_path + "/src/config-shit.h"
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
        self.perf_count = None
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
        perf_line = None
        for line in lines:
            if "rand SPEC2017" in line:
                time_line = line
            if "->" in line and self.conf.ev in line:
                perf_line = line

        assert time_line != None
        assert perf_line != None

        scratch = time_line.split(",")
        self.time = float(scratch[len(scratch) - 1].lstrip().rstrip())
        scratch = perf_line.split("->")

        self.perf_count = int(scratch[len(scratch) - 2].lstrip().rstrip())

    def get_result(self):
        assert self.has_run
        return Result(self.conf, self.time, self.perf_count)


results = {}

partialf = open(partial_file, "w+")
for ae in align_entry:
    for td in padding:
        for ev in events:
            r = Run(Config(ae, td, ev))
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
for ae in align_entry:
    for td in padding:
        for ev in events:
            res = results[make_key(ae, td, ev)]
            f.write(res.csv_out() + "\n")
f.flush()
f.close()
