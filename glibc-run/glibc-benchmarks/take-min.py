import sys

files = sys.argv[1:]
file_content = []
print(str(len(files)))
for f in files:
    fcontent = []
    for line in open(f):
        if line.count(",") < 3:
            continue
        tmp = line
        for i in range(0, 6):
            tmp = tmp[tmp.find(",") + 1:]
        dif = len(line) - len(tmp)
        fcontent.append(line)

    file_content.append(fcontent)


def cmps(e):
    s = e[1]
    s = s.split(",")
    second = max(max(float(s[6]), float(s[7])), max(float(s[8]), float(s[9])))
    return e[0] + (second / 100.0)


out = []
for i in range(0, len(file_content[0])):
    scores = []
    idx = 0
    for j in range(0, len(files)):
        score = file_content[j][i].split(",")
        score = float(score[5])

        scores.append([score, file_content[j][i]])
    scores.sort(key=cmps)
    out.append(scores[idx][1])
for i in out:
    print(i, end="")
