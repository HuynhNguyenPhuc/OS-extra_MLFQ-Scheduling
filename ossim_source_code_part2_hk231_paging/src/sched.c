
#include "queue.h"
#include "sched.h"
#include <pthread.h>

#include <stdlib.h>
#include <stdio.h>
static struct queue_t ready_queue;
static struct queue_t run_queue;
static pthread_mutex_t queue_lock;

#ifdef MLFQ_SCHED
static struct queue_t mlfq_ready_queue[MAX_PRIO];
// static int curr_prio = 0;
// static int slot_left = MAX_PRIO;
#endif

int queue_empty(void) {
#ifdef MLFQ_SCHED
	unsigned long prio;
	for (prio = 0; prio < MAX_PRIO; prio++)
		if(!empty(&mlfq_ready_queue[prio])) 
			return -1;
#endif
	return (empty(&ready_queue) && empty(&run_queue));
}

void init_scheduler(void) {
#ifdef MLFQ_SCHED
    int i;

	for (i = 0; i < MAX_PRIO; i ++)
		mlfq_ready_queue[i].size = 0;
#endif
	ready_queue.size = 0;
	run_queue.size = 0;
	pthread_mutex_init(&queue_lock, NULL);
}

#ifdef MLFQ_SCHED
/* 
 *  Stateful design for routine calling
 *  based on the priority and our MLFQ policy
 *  We implement stateful here using transition technique
 *  State representation   prio = 0 .. MAX_PRIO, curr_slot = 0..(MAX_PRIO - prio)
 */
struct pcb_t * get_mlfq_proc(void) {
	struct pcb_t * proc = NULL;
	/*TODO: get a process from PRIORITY [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	pthread_mutex_lock(&queue_lock);
    for (int i=0;i<MAX_PRIO;i++){
		if (empty(&mlfq_ready_queue[i])!=1){
			proc = dequeue(&mlfq_ready_queue[i]);
			break;
		}
	}
	pthread_mutex_unlock(&queue_lock);
	return proc;
}

void put_mlfq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	proc->prio = (MAX_PRIO-1<proc->prio+1)?(MAX_PRIO-1):(proc->prio+1);
	enqueue(&mlfq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_mlfq_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	proc->prio = 0;
	enqueue(&mlfq_ready_queue[proc->prio], proc);
	pthread_mutex_unlock(&queue_lock);	
}

struct pcb_t * get_proc(void) {
	return get_mlfq_proc();
}

void put_proc(struct pcb_t * proc) {
	return put_mlfq_proc(proc);
}

void add_proc(struct pcb_t * proc) {
	return add_mlfq_proc(proc);
}
#else
struct pcb_t * get_proc(void) {
	struct pcb_t * proc = NULL;
	/*TODO: get a process from [ready_queue].
	 * Remember to use lock to protect the queue.
	 * */
	proc = dequeue(&ready_queue);
	return proc;
}

void put_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);
}

void add_proc(struct pcb_t * proc) {
	pthread_mutex_lock(&queue_lock);
	enqueue(&ready_queue, proc);
	pthread_mutex_unlock(&queue_lock);	
}
#endif


