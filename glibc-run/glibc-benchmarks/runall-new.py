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
result_path_dir = root + "/dev-results/"
result_path = result_path_dir + "{}/"
bench_cmd = "(cd " + glibc_path + "; unset LD_LIBRARY_PATH; taskset -c 0 make --silent bench BENCHSET=\"string-benchset\")"
build_cmd_slow = "(cd " + glibc_path + "; unset LD_LIBRARY_PATH; " + root + "/src/glibc/configure --prefix=/usr; make -j 7 --silent)"
build_cmd_fast = "(cd " + glibc_path + "; unset LD_LIBRARY_PATH; make -r -C /home/noah/programs/opensource/glibc-dev/src/glibc/string/ objdir=`pwd` --silent)"
file_prefix = "bench-"
file_postfix = ".out"

from_path = root + "/dev/{}.S"
to_path = root + "/src/glibc/sysdeps/x86_64/multiarch/memmove-conf.S"

current_branch = ""


class Conf():
    def __init__(self, version, branch):
        self.version = version
        self.branch = branch

    def get_branch(self):
        multi_branch = True
        global current_branch

        if multi_branch and current_branch != self.branch:
            os.system("(cd {}; git checkout {})".format(
                git_src_path, self.branch))
            current_branch = self.branch
            return True
        return False

    def name(self):
        if self.version is None:
            return self.branch
        else:
            return "{}-{}".format(self.branch, self.version)

    def write_conf(self):
        ret = self.get_branch()

        if self.version is None:
            return ret

        content = [
            "#ifndef _BENCHMARK_CONF_H_", "#define _BENCHMARK_CONF_H_",
            "#define ALIGN_MOVSB 1", "#define LOOP_ALIGN_CONF 64",
            "#define MOVSB_ALIGN_CONF 64",
            "#define VERSION " + str(self.version), "#endif"
        ]
        f = open(to_path, "w+")
        for c in content:
            f.write(c + "\n")
        f.flush()
        f.close()
        return ret


confs = []

confs.append(Conf(None, "benchmark-dev0"))
confs.append(Conf(None, "benchmark-dev1"))
confs.append(Conf(None, "benchmark-dev2"))
confs.append(Conf(None, "benchmark-dev3"))
confs.append(Conf(None, "benchmark-dev4"))
confs.append(Conf(None, "benchmark-glibc"))


def build(conf):
    slow_build = conf.write_conf()
    if slow_build:
        print("Running: {}".format(build_cmd_slow))
        os.system(build_cmd_slow)
    else:
        print("Running: {}".format(build_cmd_fast))
        os.system(build_cmd_fast)


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


os.system("rm -rf {}; rm -rf build/glibc; mkdir -p build/glibc".format(
    result_path.format("*")))
incr = 5
for i in range(0, 1):
    for c in confs:
        build(c)
        bench(c, incr * i, incr)
