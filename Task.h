#include <unistd.h>
#include <string.h>
#include "mutex.h"

// #define MAX_NAME_LEN  32


typedef struct tcb{						/* tcb := Task Control Block */
	int pid;                   /* ID of the proces */
	int prio;                  /* process priority */
	// void (*function)(void);           	/* pointer to the process function */
	char* name;            /* Name of the process */
} Task;


void TaskInit(Task* t, int pid, int prio, char* name){
	t->pid = pid;
	t->prio = prio;
	strcpy(t->name, name);
}


char* taskGetName(Task* task){
	return task->name;
}

unsigned long taskGetId(Task* task){
	return task->pid;
}

bool taskShouldSuspend(Task* task){}

void taskSuspend(Task* task){};

void taskWait(Task* task){};

char* taskGetMem(Task* task){
	// MutexAcquire();
};

void taskReleaseMem(Task* task){
	// taskReleaseMem();
};

unsigned int taskPrio(Task* task){
	return task->prio;
};

void taskWake(pid_t p_id){};