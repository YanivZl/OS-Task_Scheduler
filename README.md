# OS-Task_Scheduler

### Summary:

Operating system that is non-preemptive and supports multi-core machines, as well as multi-threading.
The OS written in c.

### working premise:


1.  The OS is running the tasks by their priority.
2.  When SIGUSR1 signal recieved the OS starts to running the tasks by their ID.
3.  All the threads share the same address space.
4.  Each task runs the same function:
          
          do
            if taskShouldSuspend() //
            taskSuspend() // these 2 lines may be skipped
            ptr = taskGetMem()
            printf ("Cur Task is %s Last Task is %s", taskGetName(), ptr)
            taskSetMem(taskGetName()) // Set New current task
            taskReleaseMem()
            taskWait() // sleep for 1 seconds
          while (true)

3. Tasks' API:

        taskGetName - Returns a unique ascii name of the thread (It is less than 19 bytes long).
        taskGetId - Returns a unique ID.
        taskWait - Suspends a task for n seconds.
        taskGetMem - Get the buffer.
        taskSetMem - Set the buffer.
        taskReleaseMem - Release the buffer, not owner of buffer.
        taskPrio - Return the priority of the task.
        taskWake - Wake a task by its task id.

4. There is only one buffer in the entire operating system. All tasks share this buffer. The buffer size is 20 bytes.
There are NO dynamic allocations.
5. A task is a Posix thread. It is created once through the use of pthread_create.
6. Any new thread created has a unique priority. A task priority is NOT Posix priority, 
but an arbitrary number assigned to the task.
7. You may NOT use any Posix synchronization primitives.

### Compile & Run:

#### Compile:
    
Compile with Ubuntu & gcc:
         
    make all 
    or 
    gcc -pthread main.c -o mainOS
        
    
#### Run:
   
You may use 3 flags:
   
     p - Number of Proccessors/Cores (Default value 1, 1 <= value <= 4)
     n - Number of Tasks/Treads (Default value 4, 1 <= value <= 10)
     t - Tick time (Default value = 1, 1 <= value)
     
     Example:
     
     ./mainOS p3 n10 t3
     or 
     ./mainOS -p3 -n10 -t3
     
     Note: You have to use the flags separatly as shown in the example.
     
### Signal:

The miniOS supports a SIGUSR1 signal:
      
      ps -fC mainOS
      kill -s USR1 <PID>
