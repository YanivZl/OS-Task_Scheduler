#pragma once
#define __USE_GNU
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "Task.h"
#include "utils.h"


#define MEMOEY_SIZE 32

typedef struct Scheduler{
    int n_cores;
    int n_tasks;
    int tick_time;
    struct Mutex mutex;
    Task** tasks;
    char memory[MEMOEY_SIZE];

} Scheduler;

void SchedulerInit(Scheduler* scdl, int n_cores, int n_tasks, int tick_time){
    scdl->n_cores = n_cores;
    scdl->n_tasks = n_tasks;
    scdl->tick_time = tick_time;
    scdl->tasks = (Task**)malloc(n_tasks * sizeof(Task*));
    int priority_array[n_tasks];
    for (int i = 0; i < n_tasks; i++)
        priority_array[i] = i;
    shuffle(priority_array, n_tasks);
    for(int i = 0; i < n_tasks; i++){
        scdl->tasks[i] = (Task*)malloc(sizeof(Task));
        char name[8];
        snprintf(name, 8, "Task_%d", i);
        TaskInit(scdl->tasks[i], i, priority_array[i], name);
    }
    MutexInit(&scdl->mutex);
    strcpy(scdl->memory, "None");

};

void __Sort_Tasks_by_priority(Task*** p_tasks, int n_tasks){
    
    Task** temp_array = (Task**)malloc(n_tasks * sizeof(Task*));
    Task** tasks = *p_tasks;
    for (int i = 0; i < n_tasks; i++){
        temp_array[tasks[i]->prio] = tasks[i];
    }
    p_tasks = &temp_array;
    free(tasks);
}

void __Sort_Tasks_by_ID(Task*** tasks, int n_tasks){

}

void SchedulerStart(Scheduler* scdl){

    pthread_t threads[scdl->n_cores];
    pthread_attr_t at;
    cpu_set_t cpuset;
    printf("\nProgram starts with %d cores & %d threads, T value = %d.\n", scdl->n_cores, scdl->n_tasks, scdl->tick_time);
    printf("First tasks architeture:\n\n");
    printf("\tTID  |  Priority  \n");
    for(int i = 0; i < scdl->n_tasks; i++){
        printf("\t %d   |     %d   \n", scdl->tasks[i]->pid, scdl->tasks[i]->prio);
    }
    printf("\n");
    __Sort_Tasks_by_priority(&scdl->tasks, scdl->n_tasks);
    typedef struct ScheduleInfo_t{
                Task* tasks;
                char* memory;
                struct Mutex* mutex;
                int tick_time;
    } ScheduleInfo;
    while(1){
        for(int i = 0; i < scdl->n_cores; i++){
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            pthread_attr_init(&at);
            pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
            ScheduleInfo arg = {scdl->tasks[i],scdl->memory, &scdl->mutex, scdl->tick_time};
            pthread_create(&threads[i], &at, taskWake, &arg);
            // taskWake(scdl->tasks[i],scdl->memory, &scdl->mutex, scdl->tick_time);
        }
        for(int i = 0; i < scdl->n_cores; i++){
            pthread_join(threads[i], NULL);
        }
        exit(0);
    }
}
