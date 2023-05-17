#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

// #ifndef MLFQ_SCHED
// #define MLFQ_SCHED
// #endif

#define MAX_PRIO 10

int queue_empty(void);

void init_scheduler(void);
void finish_scheduler(void);

/* Get the next process from ready queue */
struct pcb_t * get_proc(void);

/* Put a process back to run queue */
void put_proc(struct pcb_t * proc);

/* Add a new process to ready queue */
void add_proc(struct pcb_t * proc);

#endif


