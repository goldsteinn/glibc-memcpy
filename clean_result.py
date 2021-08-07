import sys

fname = sys.argv[1]
content = []
for lines in open(fname):
    content.append(lines.replace("impl name               ,test name       ,size    ,al dst ,al src ,dst > src,trials ,inner trials,mean      ,median    ,geomean   ,min       ,max       ,stdev", ""))

f = open(fname, "w+")

f.write("impl name               ,test name       ,size    ,al dst ,al src ,dst > src,trials ,inner trials,mean      ,median    ,geomean   ,min       ,max       ,stdev\n")
for lines in content:
    f.write(lines)

f.flush()
f.close()
