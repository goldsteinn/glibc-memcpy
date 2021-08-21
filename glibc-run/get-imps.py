import sys

f = sys.argv[1]

for line in open(f):
    if "," in line:
        print(line.split(",")[6], end="")
    else:
        print(line, end="")
