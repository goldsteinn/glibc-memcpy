import sys

combined = ["cpu,align_entry,padding,ev_name,ref_cycles,ev_count\n"]
for to_combine in sys.argv[1:]:
    cpu = to_combine.split("-")[0]
    fname = to_combine
    for line in open(fname):
        combined.append("{},{}".format(cpu, line))

for line in combined:
    print(line, end="")
