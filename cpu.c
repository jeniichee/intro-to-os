#include "oslabs.h"

// Globals
struct PCB NULLPCB = {0, 0, 0, 0, 0, 0, 0};

// Priority-based Preemptive Scheduling (PP)
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{

    //  no current processes
    if (current_process.process_id == NULLPCB.process_id)
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    };

    // compare new process with currently-running process
    // new process prio less or equal to curr process
    if (new_process.process_priority >= current_process.process_priority)
    {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;

        // new process add to ready q
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;

        return current_process;
    };

    // new process has higher prio
    if (new_process.process_priority < current_process.process_priority)
    {

        current_process.execution_endtime = 0;
        current_process.remaining_bursttime -= timestamp - current_process.execution_starttime;

        // add current process to ready q
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    };
};

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{

    if (*queue_cnt == 0)
    {

        return NULLPCB;
    };

    int highest_prio = 0;
    for (int i = 1; i < *queue_cnt; i++)
    {

        if (ready_queue[i].process_priority < ready_queue[highest_prio].process_priority)
        {
            highest_prio = i;
        };
    };

    struct PCB next_process = ready_queue[highest_prio];

    // remove from q
    for (int i = highest_prio; i < *queue_cnt - 1; i++)
    {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;

    return next_process;
};

// Shortest-Remaining-Time-Next Preemptive Scheduling (SRTP)
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp)
{
    //  no current processes
    if (current_process.process_id == NULLPCB.process_id)
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    };

    // new process does not have shorter burst time
    if (new_process.total_bursttime >= current_process.remaining_bursttime)
    {

        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;

        // new process add to ready q
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;

        return current_process;
    };

    // new process have shorter burst time
    if (new_process.total_bursttime < current_process.remaining_bursttime)
    {
        current_process.remaining_bursttime -= timestamp - current_process.execution_starttime;
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;

        // add current process to ready q
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    };
};

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp)
{

    if (*queue_cnt == 0)
    {

        return NULLPCB;
    };

    int smallest_remaining_burstime = 0;
    for (int i = 1; i < *queue_cnt; i++)
    {

        if (ready_queue[i].remaining_bursttime < ready_queue[smallest_remaining_burstime].remaining_bursttime)
        {
            smallest_remaining_burstime = i;
        };
    };

    struct PCB next_process = ready_queue[smallest_remaining_burstime];

    // remove from q
    for (int i = smallest_remaining_burstime; i < *queue_cnt - 1; i++)
    {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;

    return next_process;
};

// Round-Robin Scheduling (RR)
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum)
{

    //  no current processes
    if (current_process.process_id == NULLPCB.process_id)
    {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;

        if (new_process.total_bursttime < time_quantum)
            new_process.execution_endtime =
                timestamp + new_process.total_bursttime;
        else
            new_process.execution_endtime =
                timestamp + time_quantum;

        return new_process;
    };

    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;

    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;

    return current_process;
};

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum)
{

    if (*queue_cnt == 0)
    {

        return NULLPCB;
    };

    int earliest = 0;
    for (int i; i < *queue_cnt; i++)
    {

        if (ready_queue[i].arrival_timestamp < ready_queue[earliest].arrival_timestamp)
        {
            earliest = i;
        };
    };

    struct PCB next_process = ready_queue[earliest];

    // remove from q
    for (int i = earliest; i < *queue_cnt - 1; i++)
    {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    next_process.execution_starttime = timestamp;

    if (next_process.remaining_bursttime < time_quantum)
        next_process.execution_endtime =
            timestamp + next_process.remaining_bursttime;
    else
        next_process.execution_endtime =
            timestamp + time_quantum;

    return next_process;
};
