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
    Task* tasks;
    char memory[MEMOEY_SIZE];

} Scheduler;

// global var for SIGUSR1, set to true when get the signal.
bool signalFlag = false;

void SchedulerInit(Scheduler* scdl, int n_cores, int n_tasks, int tick_time){
    scdl->n_cores = n_cores;
    scdl->n_tasks = n_tasks;
    scdl->tick_time = tick_time;
    scdl->tasks = (Task*)malloc(n_tasks * sizeof(Task));
    int priority_array[n_tasks];
    for (int i = 0; i < n_tasks; i++)
        priority_array[i] = i;
    shuffle(priority_array, n_tasks);
    for(int i = 0; i < n_tasks; i++){
        char name[8];
        snprintf(name, 8, "Task_%d", i);
        TaskInit(&scdl->tasks[i], i, priority_array[i], name);
    }
    MutexInit(&scdl->mutex);
    strcpy(scdl->memory, "None");

};

// Helper function
void __Sort_Tasks_by_priority(Task** p_tasks, int n_tasks){
    
    Task* temp_array = (Task*)malloc(n_tasks * sizeof(Task));
    Task* tasks = *p_tasks;
    for (int i = 0; i < n_tasks; i++){
        temp_array[n_tasks - taskPrio(&tasks[i])- 1] = tasks[i];
    }
    *p_tasks = temp_array;
}

// Strcut for sig_handler
typedef struct signal_struct_t{
    Task** tasks; 
    int n_tasks;
} Sig_Args;

// Global var for sig_handler
Sig_Args args = {};


// Helper function
void __Sort_Tasks_by_ID(){
    Task* temp_array = (Task*)malloc(args.n_tasks * sizeof(Task));
    Task* tasks = *args.tasks;
    for (int i = 0; i < args.n_tasks; i++){
        temp_array[taskGetId(&tasks[i])] = tasks[i];
    }
    *args.tasks = temp_array;
    signalFlag = true;
}

// Set function for args global var
void __set_sig_args(Task** tasks, int n_tasks){
    args.tasks = tasks;
    args.n_tasks = n_tasks;
}

// Signal Handler fot SIGUSR1
void sig_handler(int signum){

    //Return type of the handler function should be void
    if(signum == SIGUSR1){
        __Sort_Tasks_by_ID();
    printf("Get signal SIGUSR1\n");
    }
}

// Main scheduler function
void SchedulerStart(Scheduler* scdl){

    //Register a signal for SIGUSR1
    signal(SIGUSR1, sig_handler);
    __set_sig_args(&scdl->tasks, scdl->n_tasks);

    pthread_t threads[scdl->n_cores];
    pthread_attr_t at;
    cpu_set_t cpuset;
    printf("\nProgram starts with %d cores & %d threads, T value = %d.\n", scdl->n_cores, scdl->n_tasks, scdl->tick_time);
    printf("First tasks architeture:\n\n");
    printf("\tTID  |  Priority  \n");
    for(int i = 0; i < scdl->n_tasks; i++){
        printf("\t %d   |     %d   \n", taskGetId(&scdl->tasks[i]), taskPrio(&scdl->tasks[i]));
    }
    printf("\n");
    __Sort_Tasks_by_priority(&scdl->tasks, scdl->n_tasks);
    printf("\n");
    ScheduleInfo arg[scdl->n_cores];
    
    while(1){
        if(!signalFlag)
        {   
            int i = 0;
            for(; i < scdl->n_cores && i < scdl->n_tasks; i++){
                CPU_ZERO(&cpuset);
                CPU_SET(i, &cpuset);
                pthread_attr_init(&at);
                pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                arg[i] = (ScheduleInfo){&scdl->tasks[i],scdl->memory, &scdl->mutex, scdl->tick_time};
                pthread_create(&threads[i], NULL, taskWake, &arg[i]);
            }
            for(int j = 0; j < scdl->n_cores; j++){
                pthread_join(threads[j], NULL);
            }
            printf("\n");

            if(!signalFlag && scdl->n_tasks > scdl->n_cores)
            {
                for(; i < scdl->n_cores * 2; i++){
                CPU_ZERO(&cpuset);
                CPU_SET(i - scdl->n_cores, &cpuset);
                pthread_attr_init(&at);
                pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                arg[i - scdl->n_cores] = (ScheduleInfo){&scdl->tasks[i % scdl->n_tasks],scdl->memory, &scdl->mutex, scdl->tick_time};
                pthread_create(&threads[i - scdl->n_cores], NULL, taskWake, &arg[i - scdl->n_cores]);
                }
                for(int j = 0; j < scdl->n_cores; j++){
                    pthread_join(threads[j], NULL);
                }
            }
        }
        else{
            for(int i = 0; i < scdl-> n_tasks; i += scdl->n_cores)
            {
                for(int j = 0; j < scdl->n_cores; j++){
                    CPU_ZERO(&cpuset);
                    CPU_SET(j, &cpuset);
                    pthread_attr_init(&at);
                    pthread_attr_setaffinity_np(&at, sizeof(cpuset), &cpuset);
                    arg[j] = (ScheduleInfo){&scdl->tasks[(i+j) % scdl->n_tasks],scdl->memory, &scdl->mutex, scdl->tick_time};
                    pthread_create(&threads[j], NULL, taskWake, &arg[j]);
                }
                for(int j = 0; j < scdl->n_cores; j++){
                    pthread_join(threads[j], NULL);
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}
