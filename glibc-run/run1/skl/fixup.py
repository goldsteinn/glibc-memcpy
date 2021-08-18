import os
for i in range(0, 13):
    impls = []
    start = str(i) + "bench"
    for j in os.listdir("."):
        if start == j[0:len(start)]:
            impls.append(j)
    os.system("mkdir -p {}".format(i))
    for impl in impls:
        cmd = "mv {} {}/{}".format(impl, i, impl.replace(start, "bench"))
        os.system(cmd)
