#include <stdlib.h>
#include <time.h>
#include "Task.h"
#include "utils.h"

typedef struct Scheduler{
    int n_cores;
    int n_threads;
    int tick_tume;
    struct Mutex mutex;
    Task** tasks;

} Scheduler;

void SchedulerInit(Scheduler* scdl, int n_cores, int n_threads, int tick_time){
    scdl->n_cores = n_cores;
    scdl->n_threads = n_threads;
    scdl->tick_tume = tick_time;
    scdl->tasks = (Task**)malloc(n_threads * sizeof(Task*));
    int priority_array[n_threads];
    for (int i = 0; i < n_threads; i++)
        priority_array[i] = i;
    shuffle(priority_array, n_threads);
    for(int i = 0; i < n_threads; i++){
        scdl->tasks[i] = (Task*)malloc(sizeof(Task));
        char* name;
        snprintf(name, 7, "Task_%d", i);
        TaskInit(scdl->tasks[i], i, priority_array[i], name);
    }
    MutexInit(&scdl->mutex);
    
};

void SchedulerStart(Scheduler* scdl){

}
