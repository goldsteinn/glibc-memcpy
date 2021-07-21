#include "perf-ev-init.h"

#include <error-util.h>

#include <asm/unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

static int
_perf_event_open(struct perf_event_attr * ev_attr,
                 pid_t                    pid,
                 int                      cpu,
                 int                      group_fd,
                 uint64_t                 flags) {
    int ret = syscall(__NR_perf_event_open, ev_attr, pid, cpu, group_fd, flags);
    return ret;
}

static int
perf_event_open(const perf_ev_t *        perf_event,
                struct perf_event_attr * ev_attr,
                pid_t                    pid,
                int                      cpu,
                int                      group_fd,
                uint64_t                 flags) {
    int fd = _perf_event_open(ev_attr, pid, cpu, group_fd, flags);
    err_assert(fd >= 0, "Error enabling event {%x, %x, %s}\n",
               perf_event->ev_code, perf_event->type, perf_event->desc);

    return fd;
}

static void
make_ev(const perf_ev_t *        perf_event,
        struct perf_event_attr * ev_attr,
        int                      group_fd) {
    die_assert(ev_attr != NULL);
    die_assert(perf_event != NULL);
    die_assert(perf_event->type == PERF_TYPE_RAW);


    memset(ev_attr, 0, sizeof(struct perf_event_attr));
    ev_attr->type           = perf_event->type;
    ev_attr->size           = sizeof(struct perf_event_attr);
    ev_attr->config         = perf_event->ev_code;
    ev_attr->disabled       = (group_fd < 0) ? 1 : 0;
    ev_attr->exclude_kernel = 1;
    ev_attr->exclude_hv     = 1;
}

void
enable_perf(int group_fd) {
    if (group_fd == PERF_EV_INIT_SKIP) {
        return;
    }

    die_assert(group_fd > 0, "Invalid group_fd");
    err_assert(!ioctl(group_fd, PERF_EVENT_IOC_RESET, 0),
               "Failuring reset perf events");
    err_assert(!ioctl(group_fd, PERF_EVENT_IOC_ENABLE, 0),
               "Failure enabling perf events");
}

void
disable_perf(int group_fd) {
    if (group_fd == PERF_EV_INIT_SKIP) {
        return;
    }

    die_assert(group_fd > 0, "Invalid group_fd");
    err_assert(!ioctl(group_fd, PERF_EVENT_IOC_DISABLE, 0),
               "Failure enabling perf events");
}


int
_init_events(const perf_ev_t *        perf_events,
             struct perf_event_attr * ev_attrs,
             uint32_t                 nevents) {
    die_assert(nevents <= MAX_HW_EVENTS && nevents > 0,
               "Invalid number of events to track");
    make_ev(perf_events, ev_attrs, -1);
    int group_fd = perf_event_open(perf_events, ev_attrs, 0, -1, -1, 0);

    for (uint32_t i = 1; i < nevents; ++i) {
        make_ev(perf_events + i, ev_attrs + i, group_fd);
        perf_event_open(perf_events + i, ev_attrs + i, 0, -1, group_fd, 0);
    }
    return group_fd;
}

int
init_events(const perf_ev_initializer_t * initializer) {
    if (initializer == NULL || initializer->perf_events == NULL ||
        initializer->ev_attrs == NULL || initializer->nevents == 0) {
        return PERF_EV_INIT_SKIP;
    }
    return _init_events(initializer->perf_events, initializer->ev_attrs,
                        initializer->nevents);
}
