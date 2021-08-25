import os
import sys

path_fmt = "run8/skl/dev{}/bench-memcpy-random{}.out"
path_fmt = path_fmt.format(sys.argv[1], "{}")

file_contents = []
for i in range(0, 100):
    path = path_fmt.format(i)
    print(path)
    if os.access(path, os.R_OK) is False:
        break
    content = []
    markers = 0
    for line in open(path):
        if markers == 0 and "\"with-fixed-size\": 1" in line:
            markers = 1
        if markers > 0 and "\"region-size\": 196608" in line:
            markers += 1
            assert markers <= 4
        if markers == 4 and "}," in line:
            line = "}]\n}\n}\n}\n"
            content.append(line)
            break
        content.append(line)
    f = open(path, "w+")
    for line in content:
        f.write(line)
    f.flush()
    f.close()
