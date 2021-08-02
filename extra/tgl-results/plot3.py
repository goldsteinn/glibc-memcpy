import matplotlib.pyplot as plt
import csv
from pandas import DataFrame
import pandas as pd
from scipy.stats import pearsonr
import sys
import numpy as np

out = {}
assert len(sys.argv) > 1
fname = sys.argv[1]
with open(fname) as csvfile:
    header = [h.strip() for h in csvfile.readline().split(',')]
    reader = csv.DictReader(csvfile, fieldnames=header)
    for line in reader:
        n_nops = line["nops"]
        if int(n_nops) <= 32:
            if n_nops not in out:
                out[n_nops] = [[], []]
            out[n_nops][0].append(int(line["N"]))
            #out[n_nops][1].append(float(line["branch misses"]))
            #out[n_nops][1].append(float(line["lsd"]))
            #out[n_nops][1].append(float(line["branch misses"]) / float(line["lsd"]))
            out[n_nops][1].append(float(line["branch misses"]))


length_til = []
avg = []
nops = []
for key in out:
    nops.append(int(key))
    length_til.append(len(out[key][1]))
    s = 0.0
    c = 0
    for vs in out[key][1]:
        v = float(vs)
        if v > 10 * 1000 * 1000.0:
            c += 1
            s += v
    s /= c
    s /= ((16.7 * 1000 * 1000))
    avg.append(s)

    
LT = DataFrame(length_til, columns=["LT"])
AVG = DataFrame(avg, columns=["AVG"])
N = DataFrame(nops, columns=["N"])
print(min(length_til))

xticks_out = []
for i in range(2, 6):
    xticks_out.append(10 * i)

x = np.arange(len(N['N'].values))    

w = .4
plt.rcParams['xtick.labelsize'] = 8 
plt.bar(x, LT["LT"].values, width=w, label='Length Till Stabalize')
#plt.bar(x, AVG["AVG"].values, width=w, label='Avg After Spike')
plt.xticks(x, N['N'].values)
plt.ylim([0, 50])
plt.xlabel("nop")
plt.ylabel("Average After Spike")
plt.tight_layout()
plt.legend(loc='lower right')
name = sys.argv[1].split(".")[0]
plt.savefig(name + ".png", bbox_inches="tight")

plt.show()
