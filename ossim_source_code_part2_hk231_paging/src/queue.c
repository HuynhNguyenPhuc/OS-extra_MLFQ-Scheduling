#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
    if (q == NULL)
        return 1;
    return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: put a new process to queue [q] */
    if (proc == NULL || q == NULL || q->size == MAX_QUEUE_SIZE)
        return;
    q->proc[q->size] = proc;
    q->size++;
}

struct pcb_t *dequeue(struct queue_t *q)
{
    /* TODO: return a pcb whose prioprity is the highest
     * in the queue [q] and remember to remove it from q
     * */
    if (empty(q))
        return NULL;
#ifdef MLQ_SCHED
    struct pcb_t *temp = q->proc[0];
    for (int i = 0; i < q->size - 1; ++i)
    {
        q->proc[i] = q->proc[i + 1];
    }
    q->proc[q->size - 1] = NULL;
    q->size--;
    return temp;
#else
    int idx = 0;
    uint32_t max_priority = q->proc[0]->priority;
    for (int i = 1; i < q->size; i++)
    {
        if (q->proc[i]->priority > max_priority)
        {
            idx = i;
            max_priority = q->proc[i]->priority;
        }
    }

    struct pcb_t *proc = q->proc[idx];
    for (int i = idx; i < q->size - 1; i++)
        q->proc[i] = q->proc[i + 1];
    q->proc[q->size - 1] = NULL;
    q->size--;
    return proc;
#endif
}
