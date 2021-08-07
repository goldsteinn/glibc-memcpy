import os
import sys

project_path = "/home/noah/programs/projects/memcpy"
outfile = "results.txt"
tmpfile = "tmp.txt"
run_cmd = "./driver --core 0 --rt {} --func {} --min {} --max {} --scale {} --nconfs {}"


def err(s):
    print(s)
    sys.exit(-1)


class Config:
    def __init__(self, func, minv, maxv, scalev):
        self.func = func
        self.minv = minv
        self.maxv = maxv
        self.scalev = scalev
        self.nconfs = 65536

    def handle_abort(self):
        if self.nconfs == 2048:
            return False
        self.nconfs = int(self.nconfs / 2)
        return True

    def calculate_trials(self):
        return int(65536 / self.nconfs) * 10000

    def generate_cmd(self):
        return run_cmd.format(self.calculate_trials(), self.func, self.minv,
                              self.maxv, self.scalev, self.nconfs)


class Runner:
    def __init__(self, conf, first):
        self.conf = conf
        self.first = first
        self.success = False

    def run(self):
        ret = os.system(
            "(cd {}/build; make clean > /dev/null 2>&1; make > /dev/null 2>&1)"
            .format(project_path))
        assert ret == 0

        while True:
            cmd = self.conf.generate_cmd()
            print("\t\t{}".format(cmd))
            ret = os.system("(cd {}/build; {} > {} 2>&1)".format(
                project_path, cmd, tmpfile))
            if ret != 0:
                if self.conf.handle_abort() is False:
                    self.success = False
                    return False
            else:
                self.success = True
                return True

    def parse_output(self):
        if self.success is False:
            return ""

        hdr_content = ""
        csv_content = ""
        for line in open("{}/build/{}".format(project_path, tmpfile)):
            assert "error" not in line.lower() and "abort" not in line.lower()
            if "impl name" in line:
                hdr_content = line
            if "rand" in line:
                csv_content = line
                break
        assert csv_content != ""
        hdr_content = hdr_content.rstrip().lstrip()
        csv_content = csv_content.rstrip().lstrip() + "\n"
        if self.first is True:
            hdr_content += "\n"
            hdr_content = ""
        return hdr_content + csv_content


bounds = [[16, 256], [32, 256], [33, 256], [16, 257], [32, 257], [33, 257]]

no_min_maxes = [31, 32, 64, 65, 128, 256]
no_max_mins = [32, 33, 64, 65, 129, 257]

for maxes in no_min_maxes:
    bounds.append([0, maxes])
for mins in no_max_mins:
    bounds.append([mins, -1])

funcs = [
    "memcpy_dev_v32_movsb_avx2_v19", "memcpy_dev_v32_movsb_avx2_v20", "memcpy_dev_v32_movsb_avx2_v21",
    "memcpy_dev_v32_movsb_avx2_v22", "memcpy_glibc_v32_movsb_avx2"
]
scales = [1, 2, 4, 8, 16, 32, 64, 128, 256]
confs = []
for func in funcs:
    for bound in bounds:
        confs.append(Config(func, bound[0], bound[1], 1))
    for scale in scales:
        confs.append(Config(func, 0, -1, scale))

f = None
try:
    f = open(outfile, "w+")
except IOError:
    err("Unable to open file")

first = True
for repeats in range(0, 100):
    for i in range(0, len(confs)):
        print("[{:4d}] -> [{:4d} / {:4d}]:".format(repeats, i, len(confs)))

        runner = Runner(confs[i], first)
        runner.run()
        res = runner.parse_output()
        f.write(res)
        f.flush()
        first = False
f.close()
