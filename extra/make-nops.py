def make_nop_def(n):
    ind = []
    out = "#define NOP{} \\\n ".format(n)
    prev = int(int(11) * int((n - 1) / 11))
    assert prev > 0
    ind.append(prev)
    ind.append(n - prev)
    for i in range(0, len(ind) - 1):
        out += "NOP{}; \\\n".format(ind[i])
    out += "NOP{}\n".format(ind[len(ind) - 1])
    return out


for i in range(12, 1025):
    print(make_nop_def(i))
