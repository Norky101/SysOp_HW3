Group members: 
==============

	Akiva Yeshurun - 6283697 - ayesh001@fiu.edu

	Natan Farhy - 6346028 - nfarh002@fiu.edu

	Noah Pilkington - 6391207 - npilk001@fiu.edu

Section: U02

CPU Scheduling Algorithms Simulator
===================================

This program simulates various CPU scheduling algorithms using process data from an input file.

Compilation Instructions:
------------------------
To compile the program, use the following command:
    gcc skeleton_code_assignment_3.c -o cpu_scheduler

Running the Program:
------------------
1. Make sure you have the input.txt file in the same directory as the executable
2. Run the program using:
    ./cpu_scheduler
3. Follow the on-screen menu to select a scheduling algorithm:
   - Option 1: First-Come, First-Served (FCFS)
   - Option 2: Shortest Job First (SJF) - Nonpreemptive
   - Option 3: Shortest Remaining Time (SRT) - Preemptive
   - Option 4: Round Robin (RR) - You'll be prompted to enter a time quantum
   - Option 5: Priority Scheduling - Nonpreemptive
   - Option 0: Exit the program

Input File Format:
----------------
The input.txt file should follow this format:
```
Process     Burst Time      Priority        Arrival Time
====================================================
P1          6               2               0
P2          2               4               1
...
```

Notes:
-----
- Lower priority number means higher priority
- The program will display statistics including waiting time and turnaround time for each process
- Average waiting time and average turnaround time will be calculated for each algorithm
