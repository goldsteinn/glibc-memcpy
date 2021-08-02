import sys

cpu_name = "UNKOWN"


class Ev_Desc:
    def __init__(self, line):
        self.valid = False
        self.umask = None
        self.emask = None
        self.ev_name = ""

        if line == None:
            return

        assert "[" not in line and "]" not in line

        components = line.split()
        if len(components) != 2:
            return

        mask_info = components[0]
        name_info = components[1]

        if mask_info[0:4] != "0x53" or len(mask_info) != 8:
            return
        if "::" not in name_info:
            return

        global cpu_name
        cpu_name = name_info.split("::")[0]

        self.valid = True
        self.umask = mask_info[4:6]
        self.emask = mask_info[6:8]
        self.ev_name = name_info.split("::")[1].replace(":", "_")

    def make_define(self):
        if self.valid is False:
            return ""
        fmt = "#define {} [ 0x{}{}, PERF_TYPE_RAW, \"{}\" ]\n"
        ret = fmt.format(self.ev_name, self.umask, self.emask, self.ev_name)
        ret = ret.replace("[", "{").replace("]", "}")
        return ret

    def get_name(self):
        if self.valid is False:
            return ""
        return self.ev_name + "\n"

    def get_key(self):
        if self.valid is False:
            return ""
        else:
            return self.umask + self.emask


if len(sys.argv) < 2:
    print("Usage: {} {}".format(sys.argv[0], "events file"))
    sys.exit(-1)

try:
    ev_file = open(sys.argv[1], "r")
except IOError:
    print("Error opening {}".format(sys.argv[1]))
    sys.exit(-1)

ev_descs_filter = {}
for line in ev_file:
    ev_desc = Ev_Desc(line)
    if ev_desc.get_key() in ev_descs_filter:
        existing_ev_desc = ev_descs_filter[ev_desc.get_key()]
        if len(existing_ev_desc.ev_name) < len(ev_desc.ev_name):
            continue
    ev_descs_filter[ev_desc.get_key()] = ev_desc

ev_file.close()

ev_descs = []
all_events = []
for ev_desc_key in ev_descs_filter:
    ev_descs.append(ev_descs_filter[ev_desc_key])

ev_descs.sort(key=lambda x: x.ev_name.upper())

hguard_name = "_{}_EVENTS_H_".format(cpu_name.upper())
print("#ifndef {}".format(hguard_name))
print("#define {}\n".format(hguard_name))
for ev_desc in ev_descs:
    print(ev_desc.make_define(), end="")
    all_events.append(ev_desc.get_name())

print("\n#endif")

if len(sys.argv) > 2:
    try:
        list_f = open(sys.argv[2], "w+")
        for event_name in all_events:
            list_f.write(event_name)
        list_f.flush()
        list_f.close()
    except IOError:
        print("Error writing to {}".format(sys.argv[2]))
        sys.exit(-1)
