import csv
import sys
import statistics

fname = sys.argv[1]
v1 = "dev_" + sys.argv[2]
v2 = "glibc"
if len(sys.argv) > 3:
    v2 = "dev_" + sys.argv[3]
key_order = []
results = {}


def make_key(a, b, c, d):
    return str(a) + "-" + str(b) + "-" + str(c) + "-" + str(d)


def process(d):
    return statistics.median(d)


with open(fname) as csvfile:
    header = [h.strip() for h in csvfile.readline().split(',')]
    reader = csv.DictReader(csvfile, fieldnames=header)
    for line in reader:
        impl = line["impl name"].replace("memcpy_", "").replace(
            "_v32_movsb", "").lstrip().rstrip()

        size = line["size"].lstrip().rstrip()
        min_sz = line["al dst"].lstrip().rstrip()
        max_sz = line["al src"].lstrip().rstrip()
        nconfs = line["dst > src"].lstrip().rstrip()
        t = float(line["geomean"].lstrip().rstrip()) / float(line["dst > src"])
        k = make_key(size, min_sz, max_sz, nconfs)
        if k not in results:
            results[k] = {}
            key_order.append(k)

        if impl not in results[k]:
            results[k][impl] = [size, min_sz, max_sz, nconfs, [t]]
        else:
            results[k][impl][4].append(t)

print("{}, {}, {}, {}, {}, {}, {}".format("size scale", "min size", "max size",
                                          "nconfs", "new time", "cur time",
                                          "new / cur"))
for k in key_order:
    res = results[k][v1]
    td = process(results[k][v1][4])
    tg = process(results[k][v2][4])

    improve = round(100.0 * float(td / tg), 2)
    print("{}, {}, {}, {}, {}, {}, {}".format(res[0], res[1], res[2], res[3],
                                              round(td, 3), round(tg, 3),
                                              improve))
