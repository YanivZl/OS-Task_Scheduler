/*
t = taskPrio
do
    if taskShouldSuspend()
        taskSuspend()
    ptr = taskGetMem()
    printf ("Cur Task is %s Last Task is %s", taskGetName, ptr)
    SetTaskName(taskGetName()) // Set New current task
    taskReleaseMem()
    taskWait() // sleep for t seconds
while (true)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Scheduler.h"

int N = 4;      // Number of threads - 2 default.
int P = 1;      // Number of cores - 1 default.
int T = 1;      // The scheduler is invoked each T seconds (T defaults to 1, but may changed via the command line as an argument to the program).

int main(int argc, char* argv[]){

    srand(time(NULL));
    // Handle command line arguments.

    for(int i=1;i<argc;i++){
        if(strchr(argv[i], 'p') != NULL || strstr(argv[i], "-p") != NULL){
            P = atoi(strchr(argv[i], 'p') + 1);
            if(P > 4){
                printf("Input for cores is unconfirmed. Number of cores sets to MAX value 4.\n");
                P = 4;
            }
            else if(P <= 0){
                printf("Input for cores is unconfirmed. Number of cores sets to default value 1.\n");
                P = 1;
            }
        }
        if(strchr(argv[i], 'n') != NULL || strstr(argv[i], "-n") != NULL){
            N = atoi(strchr(argv[i], 'n') + 1);
            if(N > 10){
                printf("Input for threads is unconfirmed. Number of treads sets to MAX value 10.\n");
                N = 10;
            }
            else if(N <= 0){
                printf("Input for threads is unconfirmed. Number of threads sets to default value 4.\n");
                N = 4;
            }
        }
        if(strchr(argv[i], 't') != NULL || strstr(argv[i], "-t") != NULL){
            T = atoi(strchr(argv[i], 't') + 1);
            if(T <= 0){
                printf("Input for T is unconfirmed. Tick time sets to default value 1.\n");
                T = 1;
            }
        }
    }
    Scheduler scheduler;
    SchedulerInit(&scheduler, P, N, T);
    SchedulerStart(&scheduler);
    return 0;
}