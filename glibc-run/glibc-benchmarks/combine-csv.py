import sys

prefix = sys.argv[1]

fp_format = prefix + "-{}.txt"

lines = []

for l in open(fp_format.format(0)):
    line = l.lstrip().rstrip()
    if line == "" or "Results For" in line:
        continue
    stuff = line.split(",")
    lines.append(stuff[0] + "," + stuff[1] + "," + stuff[2] + "," + stuff[3] +
                 ",,," + stuff[6])

for i in range(1, 4):
    cnt = 0
    for l in open(fp_format.format(i)):
        line = l.lstrip().rstrip()
        if line == "" or "Results For" in line:
            continue
        stuff = line.split(",")[6]
        lines[cnt] += "," + stuff
        cnt += 1

for l in lines:
    print(l)
