#! /usr/bin/env python3

import os
import signal
import sys


def sig_exit(signum, empty):
    print("Exiting on Signal({})".format(str(signum)))
    sys.exit(-1)


signal.signal(signal.SIGINT, sig_exit)

funcs = {
    "memmove": ["memmove{}", "memmove-large{}", "memmove-walk{}"],
    "memcpy":
    ["memcpy{}", "memcpy-large{}", "memcpy-walk{}", "memcpy-random{}"]
}

root = "/home/noah/programs/opensource/glibc-dev"
git_src_path = root + "/src/glibc"
glibc_path = root + "/build/glibc/"
bench_path = glibc_path + "benchtests/"
result_path = root + "/dev-results/{}/"
bench_cmd = "(cd " + glibc_path + "; unset LD_LIBRARY_PATH; taskset -c 0 make --silent bench BENCHSET=\"string-benchset\")"
build_cmd = "rm -rf build/glibc; mkdir -p build/glibc; (cd " + glibc_path + "; unset LD_LIBRARY_PATH; " + root + "/src/glibc/configure --prefix=/usr; make -j 7 --silent)"

file_prefix = "bench-"
file_postfix = ".out"

from_path = root + "/dev/{}.S"
to_path = root + "/src/glibc/sysdeps/x86_64/multiarch/memmove-conf.S"

current_branch = ""


class Conf():
    def __init__(self, align_movsb, loop_align, movsb_align, version, branch):
        self.align_movsb = align_movsb
        self.loop_align = loop_align
        self.movsb_align = movsb_align
        self.version = version
        self.branch = branch

    def get_branch(self):
        global current_branch

        if current_branch != self.branch:
            os.system("(cd {}; git checkout {})".format(
                git_src_path, self.branch))
            current_branch = self.branch

    def name(self):
        if self.align_movsb is None:
            return "glibc"
        else:
            return "{}-{}-{}-{}".format(self.align_movsb, self.loop_align,
                                        self.movsb_align, self.version)

    def write_conf(self):
        self.get_branch()

        if self.align_movsb is None:
            return

        content = [
            "#ifndef _BENCHMARK_CONF_H_", "#define _BENCHMARK_CONF_H_",
            "#define ALIGN_MOVSB " + str(self.align_movsb),
            "#define LOOP_ALIGN_CONF " + str(self.loop_align),
            "#define MOVSB_ALIGN_CONF " + str(self.movsb_align),
            "#define VERSION " + str(self.version), "#endif"
        ]
        f = open(to_path, "w+")
        for c in content:
            f.write(c + "\n")
        f.flush()
        f.close()


confs = []
confs.append(Conf(1, 64, 64, 0, "benchmark-dev"))
confs.append(Conf(1, 32, 32, 0, "benchmark-dev"))
confs.append(Conf(1, 64, 64, 1, "benchmark-dev"))
confs.append(Conf(1, 32, 32, 1, "benchmark-dev"))
confs.append(Conf(None, None, None, None, "benchmark-glibc"))


def build(conf):
    conf.write_conf()
    print("Running: {}".format(build_cmd))
    os.system(build_cmd)


def bench(conf, base, inc):
    impl_result_path = result_path.format(conf.name())
    os.system("mkdir -p {}".format(impl_result_path))

    for i in range(base, base + inc):
        print("Running: {} - {}".format(conf.name(), i))
        os.system(bench_cmd)
        for func in funcs:
            impl_result_path = result_path.format(conf.name(), func)
            for f in funcs[func]:
                fname = file_prefix + f + file_postfix
                src = bench_path + fname.format("")
                dst = impl_result_path + fname.format(i)
                os.system("cp {} {}".format(src, dst))


incr = 10
for i in range(0, 1):
    for c in confs:
        build(c)
        bench(c, incr * i, incr)
