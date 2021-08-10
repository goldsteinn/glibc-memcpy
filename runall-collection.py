import os
import sys

project_path = "/home/noah/programs/projects/memcpy"
collection_path = project_path + "/collection/{}/{}.S"
impl_path = project_path + "/src/impl/core/memcpy-dev.S"
outfile = "results.txt"
tmpfile = "tmp.txt"
run_cmd = "./driver --core 0 --rt {} --func memcpy_dev_{}_movsb --min {} --max {} --scale {} --dir {} --nconfs {}"

isa = sys.argv[1]


def err(s):
    print(s)
    sys.exit(-1)


class Config:
    def __init__(self, func, minv, maxv, scalev, direction):
        self.func = func
        self.minv = minv
        self.maxv = maxv
        self.scalev = scalev
        self.direction = direction
        self.nconfs = 65536

    def handle_abort(self):
        if self.nconfs == 2048:
            return False
        self.nconfs = int(self.nconfs / 2)
        return True

    def calculate_trials(self):
        return int(65536 / self.nconfs) * 10000

    def generate_copy_cmd(self):
        return "cp {} {}".format(collection_path.format(isa, self.func), impl_path)

    def generate_cmd(self):
        return run_cmd.format(self.calculate_trials(), isa, self.minv,
                              self.maxv, self.scalev, self.direction,
                              self.nconfs)


class Runner:
    def __init__(self, conf, first):
        self.conf = conf
        self.first = first
        self.success = False
        self.has_built = False

    def build(self):
        if self.has_built is True:
            return
        copy_cmd = self.conf.generate_copy_cmd()
        ret = os.system(copy_cmd)
        assert ret == 0

        ret = os.system(
            "(cd {}/build; make clean > /dev/null 2>&1; make > /dev/null 2>&1)"
            .format(project_path))
        assert ret == 0
        self.has_built = True

    def run(self):
        self.build()
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
        hdr_content = ""
        csv_content = csv_content.rstrip().lstrip() + "\n"
        csv_content = csv_content.replace("dev", str(self.conf.func))
        if self.first is True:
            hdr_content = "impl name               ,test name       ,size    ,al dst ,al src ,dst > src,trials ,inner trials,mean      ,median    ,geomean   ,min       ,max       ,stdev\n"

        return hdr_content + csv_content


bounds = [[16, 256], [32, 256], [33, 256], [16, 257], [32, 257], [33, 257]]

no_min_maxes = [31, 32, 64, 65, 128, 256]
no_max_mins = [16, 32, 33, 64, 65, 129, 257]

for maxes in no_min_maxes:
    bounds.append([0, maxes])
for mins in no_max_mins:
    bounds.append([mins, -1])

funcs = []
for i in range(0, 100):
    if os.path.isfile(collection_path.format(isa, i)):
        funcs.append(i)

scales = [1]
directions = [0, 1, 2]
confs = []
for func in funcs:
    for scale in scales:
        for direction in directions:
            confs.append(Config(func, 0, -1, scale, direction))

            c = Config(func, 0, -1, scale, direction)
            c.nconfs = 8192
            confs.append(c)

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
