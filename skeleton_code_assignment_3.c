// Add more header files if required...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_PROCESSES 100

/* ========================================================================================*/
// Structure to store process information
typedef struct
{ // Add more variables to store process information if required...
    int pid;
    int priority;
    int burst_time;
    int arrival_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    bool is_completed;

} Process;

/* ========================================================================================*/
// Add more Global variables if required...
Process processes[MAX_PROCESSES];
int num_processes = 0;

/* ========================================================================================*/
// Add more Function prototypes suitable your implementation if required...
void fcfs();                        // First Come First Serve Scheduling
void sjf_non_preemptive();          // Shortest Job First - Non-preemptive
void srt_preemptive();              // Shortest Remaining Time - Preemptive
void round_robin(int time_quantum); // Round Robin Scheduling
void priority_non_preemptive();     // Priority Scheduling - Non-preemptive
void read_processes_from_file(const char *filename); //Read and process the process-related information form `input.txt` file
void calculate_average_times();     // Calculate and display average waiting & turnaround times
void display_results();             // Display scheduling results
void display_process_info();        // Display process details before scheduling
void clear_input_buffer();          // Clears input buffer to prevent invalid input issues
void reset_process_states();        // Resets process variables before each scheduling run
int min(int a, int b);              // Utility function to find the minimum of two numbers

/* ========================================================================================*/
// Main function
int main()
{
    // Add more variables suitable for your implementation if required...
    int choice;
    int time_quantum;
    char input[100]; // Buffer to store user input

    // Read process data from file
    read_processes_from_file("input.txt");

    // User-driven menu
    while (1)
    {
        printf("\n                 CPU Scheduling Algorithms\n");
        printf("|-----------------------------------------------------------|\n");
        printf("|   1. First-Come, First-Served (FCFS)                      |\n");
        printf("|   2. Shortest Job First (SJF) - Nonpreemptive             |\n");
        printf("|   3. Shortest Remaining Time (SRT) - Preemptive           |\n");
        printf("|   4. Round Robin (RR)                                     |\n");
        printf("|   5. Priority Scheduling - Nonpreemptive                  |\n");
        printf("|   0. Exit                                                 |\n");
        printf("|-----------------------------------------------------------|\n");

        printf("\nEnter your choice: ");
        if (!fgets(input, sizeof(input), stdin))
        {
            // Handle EOF (e.g., Ctrl+D)
            printf("\nExiting program.\n\n");
            break;
        }

        // Validate input: check if it's an integer
        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input. Please enter an integer between 0 and 5.\n");
            continue;
        }

        printf("\n");

        switch (choice)
        {
        case 1:
            fcfs();
            break;
        case 2:
            sjf_non_preemptive();
            break;
        case 3:
            srt_preemptive();
            break;
        case 4:
            printf("Enter time quantum for Round Robin Scheduling: ");
            printf("\n");
            scanf("%d", &time_quantum);
            clear_input_buffer(); // Clear the buffer after reading input
            round_robin(time_quantum);
            break;
        case 5:
            priority_non_preemptive();
            break;
        case 0:
            printf("Exiting program.\n\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

/* ========================================================================================*/
// Function to read processes from a file (PLEASE DONOT MODIFY THIS FUNCTION CODE!!!)
void read_processes_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Buffer to hold each line from the file
    char line[256];
    int line_number = 0;

    // Skip the first two lines (header and separator)
    while (line_number < 2 && fgets(line, sizeof(line), file))
    {
        line_number++;
    }

    // Read process data from the file
    while (fgets(line, sizeof(line), file))
    {
        // Skip lines with separators like "====" or "----"
        if (line[0] == '=' || line[0] == '-')
        {
            continue;
        }

        char pid_str[10]; // Buffer to store process name like "P1" or "1"
        int pid, priority, burst_time, arrival_time;

        // Read the process ID and other values
        if (sscanf(line, "%s %d %d %d", pid_str, &burst_time, &priority, &arrival_time) == 4)
        {
            // Extract numeric part from 'P1' or read directly if it's just '1'
            if (sscanf(pid_str, "P%d", &pid) != 1)
            { // Check if it starts with 'P'
                if (sscanf(pid_str, "%d", &pid) != 1)
                { // Otherwise, try reading as a number
                    printf("Invalid process ID format: %s (skipped)\n", pid_str);
                    continue; // Skip invalid process IDs
                }
            }

            // Store the process data
            processes[num_processes].pid = pid;
            processes[num_processes].priority = priority;
            processes[num_processes].burst_time = burst_time;
            processes[num_processes].arrival_time = arrival_time;
            processes[num_processes].remaining_time = burst_time; // Remaining time equals burst time initially
            processes[num_processes].waiting_time = 0;            // Initialize waiting time for aging
            num_processes++;                                      // Increment process count
        }
        else
        {
            printf("Invalid line format: %s (skipped)\n", line); // Handle invalid data lines
        }
    }

    // Close the file after reading
    fclose(file);

    // Display the loaded process information
    display_process_info();
}

/* ========================================================================================*/
// Function to reset process states
void reset_process_states()
{
    // Also reset other process information variables if you have added to the 'Process' structure...
    for (int i = 0; i < num_processes; i++)
    {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].is_completed = false;
    }
}

/* ========================================================================================*/
// Function to clear the input buffer (PLEASE DO NOT MODIFY THIS FUNCTION CODE!!!)
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Consume characters until newline or EOF
}

/* ========================================================================================*/
// Function to display the scheduling statistics
void display_process_info()
{
    // Print fetched values in a table format
    printf("\n\n             Process Scheduling Information\n");
    printf("----------------------------------------------------------\n");
    printf("  | %-5s | %-12s | %-12s | %-12s |\n", "PID", "Burst Time", "Priority", "Arrival Time");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < num_processes; i++)
    {
        printf("  | %-5d | %-12d | %-12d | %-12d |\n",
               processes[i].pid, processes[i].burst_time,
               processes[i].priority, processes[i].arrival_time);
    }

    printf("----------------------------------------------------------\n\n");
}

/* ========================================================================================*/
// Finding minmum of two numbers
int min(int a, int b)
{
    return (a < b) ? a : b;
}

/* ========================================================================================*/
// Calculate Average Waiting Time & Average Turnaround Time
void calculate_average_times()
{
    // Variables to store total waiting and turnaround times
    int wait_sum = 0;
    int ta_sum = 0;
    
    // Loop through all processes and add up their times
    for (int i = 0; i < num_processes; i++) {
        // Add this process's waiting time to total
        wait_sum += processes[i].waiting_time;
        
        // Add this process's turnaround time to total
        ta_sum += processes[i].turnaround_time;
        
        // Debugging - removed for final submission
        // printf("P%d: WT=%d, TAT=%d\n", processes[i].pid, 
        //        processes[i].waiting_time, processes[i].turnaround_time);
    }
    
    // Calculate averages
    // Need to cast to float to get decimal results
    float avg_wait = (float)wait_sum / num_processes;
    float avg_ta = (float)ta_sum / num_processes;

    // Print results with 2 decimal places
    printf("\nAverage Waiting Time: %.2f\n", avg_wait);
    printf("\nAverage Turnaround Time: %.2f\n", avg_ta);
}

/* ========================================================================================*/
// Display results
void display_results()
{
    // Display Individual Process Turnaround Time & Waiting Time
    printf("\nProcess\t   Waiting Time\t    Turnaround Time\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("  P%d\t       %d\t\t %d\n", processes[i].pid, processes[i].waiting_time, processes[i].turnaround_time);
    }

    // Display Average Waiting Time & Average Turnaround Time
    calculate_average_times();
}

/* ========================================================================================*/
// First-Come, First-Served (FCFS) Scheduling
void fcfs()
{
    // Reset process states before execution
    reset_process_states();

    // Sort processes by arrival time using bubble sort
    // I tried using qsort first but had issues with the compare function
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap the processes
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Initialize current time to 0
    int curr_time = 0;
    
    // Process each job in order of arrival
    for (int i = 0; i < num_processes; i++) {
        // CPU idle until next process arrives
        if (curr_time < processes[i].arrival_time) {
            curr_time = processes[i].arrival_time;
        }
        
        // Calculate waiting time for this process
        // Waiting time = time when process starts - arrival time
        processes[i].waiting_time = curr_time - processes[i].arrival_time;
        
        // Execute the process (add burst time to current time)
        curr_time = curr_time + processes[i].burst_time;
        
        // Update completion time and calculate turnaround time
        processes[i].completion_time = curr_time;
        // Turnaround time = completion time - arrival time
        processes[i].turnaround_time = curr_time - processes[i].arrival_time;
        processes[i].is_completed = true;
        
        // Debug print - removed for final submission
        // printf("Process %d completed at time %d\n", processes[i].pid, curr_time);
    }

    printf("***************************************************************************************\n\n");
    printf("FCFS Statistics...\n");
    display_results(); // Display Statistics
    printf("\n***************************************************************************************\n");
}

/* ========================================================================================*/
// Shortest Job First (SJF) - Non-Preemptive
void sjf_non_preemptive()
{
    // Reset process states
    reset_process_states();

    int time = 0; // current time
    int done = 0; // number of completed processes
    int i, j;     // loop variables
    
    // My implementation of SJF non-preemptive
    // Had to rewrite this a few times to get it right
    
    while (done < num_processes) {
        // Find shortest job that has arrived
        int shortest = -1;  // index of shortest job
        int min_burst = 9999; // some large number
        
        // Check all processes
        for (i = 0; i < num_processes; i++) {
            // Process must have arrived and not be completed
            if (processes[i].arrival_time <= time && 
                processes[i].is_completed == false) {
                
                // If this process has shorter burst time than current shortest
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    shortest = i;
                }
            }
        }
        
        // No eligible process found, just increment time
        if (shortest == -1) {
            time++;
            continue; // go to next iteration
        }
        
        // Found a process to execute
        // Calculate waiting time (current time - arrival time)
        processes[shortest].waiting_time = time - processes[shortest].arrival_time;
        
        // Update completion time
        processes[shortest].completion_time = time + processes[shortest].burst_time;
        
        // Calculate turnaround time
        processes[shortest].turnaround_time = processes[shortest].completion_time - 
                                             processes[shortest].arrival_time;
        
        // Mark as completed
        processes[shortest].is_completed = true;
        
        // Jump time to completion of this process
        time = processes[shortest].completion_time;
        
        // Increment completed processes counter
        done++;
    }

    // Display results
    printf("***************************************************************************************\n\n");
    printf("SJF (Non-Preemptive) Statistics...\n");
    display_results();
    printf("\n***************************************************************************************\n\n");
}

/* ========================================================================================*/
// Shortest Remaining Time (SRT) - SJF Preemptive
void srt_preemptive()
{
    // Reset process states before execution
    reset_process_states();

    // This one was tricky to implement
    // Had to debug a lot to get it working correctly
    
    int t = 0;          // current time
    int finished = 0;   // number of completed processes
    int last_proc = -1; // last process that was executed
    
    // Need to track when each process started execution
    // This helps calculate waiting time correctly
    int start[MAX_PROCESSES];
    memset(start, 0, sizeof(start)); // initialize all to 0
    
    // Main loop - continue until all processes complete
    while (finished < num_processes) {
        
        // Find process with minimum remaining time
        int next_proc = -1;
        int min_rem = 999999; // some large number
        
        for (int i = 0; i < num_processes; i++) {
            // Check if process has arrived and is not completed
            if (processes[i].arrival_time <= t && 
                processes[i].is_completed == false) {
                
                // Check if this process has less remaining time
                if (processes[i].remaining_time < min_rem) {
                    min_rem = processes[i].remaining_time;
                    next_proc = i;
                }
            }
        }
        
        // If no process found, just increment time
        if (next_proc == -1) {
            t = t + 1;
            continue;
        }
        
        // Context switch - different process from last time
        if (last_proc != next_proc) {
            // Record start time for this process
            start[next_proc] = t;
            last_proc = next_proc;
        }
        
        // Execute process for 1 time unit
        processes[next_proc].remaining_time--;
        t++; // increment time
        
        // Check if process is now complete
        if (processes[next_proc].remaining_time == 0) {
            // Process is complete
            processes[next_proc].completion_time = t;
            
            // Calculate turnaround time
            // Turnaround = completion - arrival
            int ta_time = processes[next_proc].completion_time - 
                         processes[next_proc].arrival_time;
            processes[next_proc].turnaround_time = ta_time;
            
            // Calculate waiting time
            // Waiting = turnaround - burst
            processes[next_proc].waiting_time = ta_time - 
                                              processes[next_proc].burst_time;
            
            // Mark as completed
            processes[next_proc].is_completed = true;
            finished++;
            
            // Need to find a new process
            last_proc = -1;
        }
    }

    printf("***************************************************************************************\n\n");
    printf("SRT (Preemptive) Statistics...\n");
    display_results();
    printf("\n***************************************************************************************\n\n");
}

/* ========================================================================================*/
// Round Robin (RR) Scheduling
void round_robin(int time_quantum)
{
    // Reset process states before execution
    reset_process_states();

    // Sort processes by arrival time first
    // This is important for RR to work correctly
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap
                Process tmp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = tmp;
            }
        }
    }

    // Variables for RR algorithm
    int clock = 0;      // Current time
    int complete = 0;   // Number of completed processes
    
    // Ready queue implementation
    int ready_q[100];   // Queue of process indices
    int front = 0;      // Front of queue
    int rear = 0;       // Rear of queue
    
    // Add first process to queue
    ready_q[rear] = 0;
    rear++;
    
    // Keep track of which processes are in queue to avoid duplicates
    int in_q[MAX_PROCESSES];
    for (int i = 0; i < MAX_PROCESSES; i++) {
        in_q[i] = 0;  // 0 means not in queue
    }
    in_q[0] = 1;      // First process is in queue
    
    // Main loop - continue until all processes complete
    while (complete < num_processes) {
        
        // Check if queue is empty
        if (front == rear) {
            // Queue is empty, increment time and check for new arrivals
            clock++;
            
            // Check if any new processes have arrived
            for (int i = 0; i < num_processes; i++) {
                if (processes[i].arrival_time <= clock && 
                    !processes[i].is_completed && in_q[i] == 0) {
                    
                    // Add to queue
                    ready_q[rear] = i;
                    rear++;
                    in_q[i] = 1;
                }
            }
            continue;  // Go back to while loop
        }
        
        // Get process from front of queue
        int current_proc = ready_q[front];
        front++;
        in_q[current_proc] = 0;  // No longer in queue
        
        // Calculate how long this process will run
        // Either time quantum or remaining time, whichever is smaller
        int run_time;
        if (processes[current_proc].remaining_time <= time_quantum) {
            run_time = processes[current_proc].remaining_time;
        } else {
            run_time = time_quantum;
        }
        
        // Run the process for run_time
        processes[current_proc].remaining_time -= run_time;
        clock += run_time;
        
        // Check for new arrivals during this time slice
        for (int i = 0; i < num_processes; i++) {
            // Skip current process and processes already in queue
            if (i == current_proc || in_q[i] == 1) {
                continue;
            }
            
            // Add newly arrived processes to queue
            if (processes[i].arrival_time <= clock && 
                !processes[i].is_completed) {
                
                ready_q[rear] = i;
                rear++;
                in_q[i] = 1;
            }
        }
        
        // Check if process is complete
        if (processes[current_proc].remaining_time == 0) {
            // Process is complete
            processes[current_proc].completion_time = clock;
            processes[current_proc].turnaround_time = 
                processes[current_proc].completion_time - 
                processes[current_proc].arrival_time;
            processes[current_proc].waiting_time = 
                processes[current_proc].turnaround_time - 
                processes[current_proc].burst_time;
            processes[current_proc].is_completed = true;
            complete++;
        } else {
            // Process not complete, add back to queue
            ready_q[rear] = current_proc;
            rear++;
            in_q[current_proc] = 1;
        }
    }

    printf("***************************************************************************************\n\n");
    printf("RR Statistics (Time Quantum = %d)...\n", time_quantum);
    display_results(); // Display Statistics
    printf("\n***************************************************************************************\n");
}

/* ========================================================================================*/
// Priority Scheduling - Non-Preemptive
void priority_non_preemptive()
{
    // Reset process states before execution
    reset_process_states();

    // Variables for priority scheduling
    int now = 0;        // Current time
    int count = 0;      // Count of completed processes
    
    // I initially tried to sort processes by priority but that doesn't work
    // because we need to consider arrival times too
    
    // Main scheduling loop
    while (count < num_processes) {
        int best_idx = -1;
        int best_priority = 999; // Lower number = higher priority
        
        // Find highest priority process that has arrived
        for (int i = 0; i < num_processes; i++) {
            // Skip completed processes
            if (processes[i].is_completed == true) {
                continue;
            }
            
            // Check if process has arrived
            if (processes[i].arrival_time <= now) {
                // Check if this process has higher priority (lower number)
                if (processes[i].priority < best_priority) {
                    best_priority = processes[i].priority;
                    best_idx = i;
                }
            }
        }
        
        // If no process found, increment time
        if (best_idx == -1) {
            now++;
            continue;
        }
        
        // Execute the highest priority process
        // Calculate waiting time
        processes[best_idx].waiting_time = now - processes[best_idx].arrival_time;
        
        // Execute process (add burst time to current time)
        now += processes[best_idx].burst_time;
        
        // Update completion time
        processes[best_idx].completion_time = now;
        
        // Calculate turnaround time
        processes[best_idx].turnaround_time = 
            processes[best_idx].completion_time - processes[best_idx].arrival_time;
        
        // Mark as completed
        processes[best_idx].is_completed = true;
        
        // Increment completed count
        count++;
        
        // Debug - removed for final submission
        // printf("Executed P%d with priority %d at time %d\n", 
        //        processes[best_idx].pid, processes[best_idx].priority, now);
    }

    printf("***************************************************************************************\n\n");
    printf("Priority (PR) - Nonpreemptive Statistics...\n");
    display_results(); // Display Statistics
    printf("\n***************************************************************************************\n");
}
