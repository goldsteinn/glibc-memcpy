#! /usr/bin/env python3

import os
import signal
import sys

N = int(sys.argv[1])


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
glibc_path = root + "/build/glibc/"
bench_path = glibc_path + "benchtests/"
result_path = root + "/dev-results/{}"
bench_cmd = "(cd " + glibc_path + "; unset LD_LIBRARY_PATH; taskset -c 0 make --silent bench BENCHSET=\"string-benchset\")"
build_cmd = "rm -rf build; mkdir -p build/glibc; (cd " + glibc_path + "; unset LD_LIBRARY_PATH; " + root + "/src/glibc/configure --prefix=/usr; make -j 7 --silent)"

file_prefix = "bench-"
file_postfix = ".out"

from_path = root + "/dev/{}.S"
to_path = root + "/src/glibc/sysdeps/x86_64/multiarch/memmove-vec-dev.S"


def copy(impl, fake):
    full_from_path = from_path.format(impl)
    full_to_path = to_path
    print("Running: " + "cp {} {}".format(full_from_path, full_to_path))
    if fake is False:
        assert os.system("cp {} {}".format(full_from_path, full_to_path)) == 0


def build(impl, t):
    copy(impl, False)
    print("Running: {}".format(build_cmd))
    os.system(build_cmd)


def bench(impl, base, inc):
    for func in funcs:
        impl_result_path = result_path.format(impl, func)
        os.system("mkdir -p {}".format(impl_result_path))

    for i in range(base, base + inc):
        print("Running: {} - {}".format(impl, i))
        os.system(bench_cmd)
        for func in funcs:
            impl_result_path = result_path.format(impl, func)
            for f in funcs[func]:
                fname = file_prefix + f + file_postfix
                src = bench_path + fname.format("")
                dst = impl_result_path + fname.format(i)
                os.system("cp {} {}".format(src, dst))


incr = 10
for i in range(0, 1):
    for j in range(0, N):
        build(j, i)
        bench(j, incr * i, incr)
