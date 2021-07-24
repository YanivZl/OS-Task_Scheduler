#include <unistd.h>
#include <string.h>
#include "mutex.h"

#define MAX_NAME_LEN  20


typedef struct tcb{						
	int pid;                   			/* ID of the task */
	int prio;                  			/* task priority */
	char name[MAX_NAME_LEN];            /* Name of the task */
} Task;


void TaskInit(Task* t, int pid, int prio, char* name){
	t->pid = pid;
	t->prio = prio;
	strcpy(t->name, name);
};


char* taskGetName(Task* task){
	return task->name;
};

unsigned long taskGetId(Task* task){
	return task->pid;
};

// bool taskShouldSuspend(Task* task){
// 
// };

// void taskSuspend(Task* task){
// 	
// };

void taskWait(int t_sleep){
	sleep(t_sleep);
};

void taskGetMem(struct Mutex* m){
	MutexAcquire(m);
};

void taskSetMem(char* memory, char* new_memory){
	strcpy(memory, new_memory);
};

void taskReleaseMem(struct Mutex* m){
	MutexRelease(m);
};

unsigned int taskPrio(Task* task){
	return task->prio;
};


typedef struct ScheduleInfo_t{
                Task* task;
                char* memory;
                struct Mutex* mutex;
                int tick_time;
    } ScheduleInfo;

void* taskWake(void* args){

	ScheduleInfo* sc_info = (ScheduleInfo*)args;
	// if taskShouldSuspend(cur_task*){
		// taskSuspend(cur_task);
	// }
	taskGetMem(sc_info->mutex);
	printf("Cur Task is %s Last Task is %s\n\n", taskGetName(sc_info->task), sc_info->memory);
	taskSetMem(sc_info->memory, taskGetName(sc_info->task));
	taskReleaseMem(sc_info->mutex);
	taskWait(sc_info->tick_time);
	return NULL;
};