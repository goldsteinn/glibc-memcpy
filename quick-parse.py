import csv
import sys
import statistics

fname = sys.argv[1]
key_order = []
results = {}


def make_key(a, b, c):
    return str(a) + "-" + str(b) + "-" + str(c)

def process(d):
    return statistics.median(d)
    
with open(fname) as csvfile:
    header = [h.strip() for h in csvfile.readline().split(',')]
    reader = csv.DictReader(csvfile, fieldnames=header)
    for line in reader:
        impl = line["impl name"]
        if "glibc" in impl:
            impl = "glibc"
        else:
            impl = "dev"

        size = line["size"].lstrip().rstrip()
        min_sz = line["al dst"].lstrip().rstrip()
        max_sz = line["al src"].lstrip().rstrip()
        t = float(line["geomean"].lstrip().rstrip()) / float(line["dst > src"])
        k = make_key(size, min_sz, max_sz)
        if k not in results:
            results[k] = {}
            key_order.append(k)

        if impl not in results[k]:
            results[k][impl] = [size, min_sz, max_sz, [t]]
        else:
            results[k][impl][3].append(t)

print("{}, {}, {}, {}, {}, {}".format("size scale", "min size", "max size",
                                      "new time", "cur time", "new / cur"))
for k in key_order:
    res = results[k]["dev"]
    td = process(results[k]["dev"][3])
    tg = process(results[k]["glibc"][3])

    improve = round(100.0 * float(td / tg), 2)
    print("{}, {}, {}, {}, {}, {}".format(res[0], res[1], res[2], round(td, 3),
                                          round(tg, 3), improve))
