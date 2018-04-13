# List all of the main states a process may be in at any point in time on a standard Unix system. Briefly explain what each of these states mean.
Running: Process has all of its resources and CPU. Indicated by state flag R in process list.

Runnable: Process has all of its resources but no CPU; waiting for CPU availability to switch to running state. Also indicated by state flag R in the process list.

Sleeping: Process is waiting for resources so that it can run; gives up CPU while it waits. Sleep mode is either Interruptible (S in process list) or Uninterruptible (D in process list). Interruptible sleep mode allows it to exit sleep mode when when the necessary conditions occur; Uninterruptible sleep mode ends when its pre-set timer expires or waited-upon resources become available.

Stopped: Process ends or terminates after receiving a kill signal or entering exit status. All resources are released but it remains in the process table and signals parent process that it has terminated, which allows the parent to examine how the child exited and take action to release it from the process table.

Zombie: This is the state the child process is in between when it sends the terminated signal to the parent and when the parent clearing it from the process table as described in the "Stopped" state above. A child process can remain in zombie state if its parent process is terminated before it releases the child process. Zombie state is indicated by Z in the process list.

# What is a Zombie Process? How does it get created? How does it get destroyed?
Explained in the "Stopped" and "Zombie" states above. When a process receives a kill signal or terminates, it enters Stopped status and notifies it's parent, then enters Zombie status. It remains in Zombie status until the parent process releases it from the process table. If the parent process is terminated before that happens, the child process remains in a Zombie state.

# Describe the job of the Scheduler in the OS in general.
The scheduler allocates system resources to the many tasks being handled by the OS. It handles the job queues and determines which job should be run next from which queue and how much time it should run.

# Describe the benefits of the MLFQ over a plain Round-Robin scheduler.
MLFQ allows for prioritization of jobs while round-robin does not. In a round-robin scheduler, jobs run in a first-in-first-out sequence for a quantum of time before allowing the next job to run, regardless of any sense of priority between the jobs. MLFQ implements prioritization through use of multiple queues and movement of a job from higher to lower priority the longer it takes to complete, thereby allowing shorter jobs to complete faster and get out of the way.