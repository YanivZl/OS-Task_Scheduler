all : main.c Scheduler.h Task.h utils.h mutex.h
	gcc -pthread main.c -o mainOS