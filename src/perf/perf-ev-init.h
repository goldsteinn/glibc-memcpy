#ifndef _PERF_EV_INIT_H_
#define _PERF_EV_INIT_H_
#include <stdint.h>
#include <linux/perf_event.h>

static const uint32_t MAX_HW_EVENTS     = 7;
static const uint32_t PERF_EV_INIT_SKIP = 0;
typedef struct perf_ev {
    uint32_t     ev_code;
    uint32_t     type;
    const char * desc;
} perf_ev_t;

typedef struct perf_ev_initializer {
    const perf_ev_t *        perf_events;
    struct perf_event_attr * ev_attrs;
    uint32_t                 nevents;
} perf_ev_initializer_t;

void enable_perf(int group_fd);
int  init_events(const perf_ev_initializer_t * initializer);


#endif
