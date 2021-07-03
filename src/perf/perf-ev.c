#include <error-util.h>

#include <asm/unistd.h>
#include <fcntl.h>
#include <linux/perf_event.h>
#include <perfmon/pfmlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

static int pfm_is_init = 0;

static int
perf_event_open(struct perf_event_attr * ev_attr,
                pid_t                    pid,
                int                      cpu,
                int                      group_fd,
                uint64_t                 flags) {
    int ret = syscall(__NR_perf_event_open, ev_attr, pid, cpu, group_fd, flags);
    return ret;
}

void
manual_init(uint64_t ev_code, struct perf_event_attr * ev_attr) {
    die_assert(ev_attr != NULL);
    memset(ev_attr, 0, sizeof(struct perf_event_attr));
    ev_attr->type           = PERF_TYPE_RAW;
    ev_attr->size           = sizeof(struct perf_event_attr);
    ev_attr->config         = ev_code;
    ev_attr->disabled       = 1;
    ev_attr->exclude_kernel = 1;
    ev_attr->exclude_hv     = 1;
}

static void
pfm_ensure_init() {
    if (UNLIKELY(pfm_is_init == 0)) {
        err_assert(pfm_initialize() == PFM_SUCCESS);
    }
}
#if 0
void
pfm_init(const char * str, int dfl_plm, struct perf_event_attr * ev_attr) {
    pfm_ensure_init();

    pfm_perf_encode_arg_t ev_init;
    memset(&ev_init, 0, sizeof(pfm_perf_encode_arg_t));

    ev_init.attr = ev_attr;
    ev_init.size = sizeof(pfm_perf_encode_arg_t);
    err_assert(pfm_get_os_event_encoding("RETIRED INSTRUCTIONS", -1,
                                         PFM_OS_PERF_EVENT,
                                         &arg) == PFMLIB_SUCCESS);
}
#endif
int
open_events(struct perf_event_attr * ev_attrs,
            uint64_t                 nevents,
            uint32_t                 core) {
    die_assert(nevents && ev_attrs);
    ev_attrs[0].disabled = 1;
    int group_fd         = perf_event_open(ev_attrs, 0, core, -1, 0);
    err_assert(group_fd >= 0);
    for (uint64_t i = 1; i < nevents; ++i) {
        err_assert(perf_event_open(ev_attrs + i, 0, core, group_fd, 0) >= 0);
    }
    return group_fd;
}

void
enable_perf(int group_fd) {
    err_assert(!ioctl(group_fd, PERF_EVENT_IOC_RESET, 0),
               "Failuring reset perf events");
    err_assert(!ioctl(group_fd, PERF_EVENT_IOC_ENABLE, 0),
               "Failure enabling perf events");
}
