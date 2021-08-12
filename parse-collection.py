import csv
import sys
import statistics

fname = sys.argv[1]
v1 = "{}_" + sys.argv[2]
v2 = v1

v1 = v1.format(sys.argv[3])
v2 = v2.format(sys.argv[4])

key_order = []
results = {}


def to_str(d):
    d = int(d)
    if d == 0:
        return "FORWARD"
    if d == 1:
        return "BACKWARD"
    return "BIDIRECTIONAL"


def make_key(a, b, c, d, e):
    return str(a) + "-" + str(b) + "-" + str(c) + "-" + str(d) + "-" + str(e)


def process(d):
    return statistics.median(d)


with open(fname) as csvfile:
    header = [h.strip() for h in csvfile.readline().split(',')]
    reader = csv.DictReader(csvfile, fieldnames=header)
    for line in reader:
        impl = line["impl name"].replace("memcpy_",
                                         "").replace("_movsb",
                                                     "").lstrip().rstrip()

        size = line["size"].lstrip().rstrip()
        min_sz = line["al dst"].lstrip().rstrip()
        max_sz = line["al src"].lstrip().rstrip()
        nconfs = line["inner trials"].lstrip().rstrip()
        direction = line["dst > src"].lstrip().rstrip()
        t = float(line["geomean"].lstrip().rstrip()) / float(nconfs)
        k = make_key(size, min_sz, max_sz, direction, nconfs)
        if k not in results:
            results[k] = {}
            key_order.append(k)

        if impl not in results[k]:
            results[k][impl] = [size, min_sz, max_sz, direction, nconfs, [t]]
        else:
            results[k][impl][5].append(t)

full = False
if full:
    print("{0:10}, {1:8}, {2:8}, {3:13}, {4:6}, {5:8}, {6:8}, {7:5}".format(
        "size scale", "min size", "max size", "direction", "nconfs",
        "new time", "cur time", "new / cur"))
else:
    print("{0:13}, {1:8}, {2:8}".format("direction", "nconfs", "new / cur"))

for k in key_order:
    res = results[k][v1]
    td = process(results[k][v1][5])
    tg = process(results[k][v2][5])

    improve = round(100.0 * float(td / tg), 2)
    if full:
        print(
            "{0:10}, {1:8}, {2:8}, {3:13}, {4:6}, {5:8}, {6:8}, {7:5}".format(
                res[0], res[1], res[2], to_str(res[3]), res[4], round(td, 3),
                round(tg, 3), improve))
    else:
        print("{0:13}, {1:8}, {2:8}".format(to_str(res[3]), res[4], improve))
