#include <iostream>
#include <vector>
#include <climits>
using namespace std;

// Structure to represent a process
struct Process {
    int id;             // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst time
    int remainingTime;  // Remaining burst time (for preemptive scheduling)
    int priority;       // Priority (lower number means higher priority)
    int completionTime; // Completion time
    int turnaroundTime; // Turnaround time (TAT = Completion Time - Arrival Time)
    int waitingTime;    // Waiting time (WT = TAT - Burst Time)
};

// Function for Priority Non-Preemptive Scheduling
void priorityNonPreemptive(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> isCompleted(n, false);
    int currentTime = 0, completed = 0;

    while (completed < n) {
        int selectedProcess = -1, highestPriority = INT_MAX;

        // Select process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrivalTime <= currentTime &&
                processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                selectedProcess = i;
            }
        }

        if (selectedProcess == -1) {
            currentTime++; // Increment time if no process is ready
            continue;
        }

        // Process execution
        currentTime += processes[selectedProcess].burstTime;
        processes[selectedProcess].completionTime = currentTime;
        processes[selectedProcess].turnaroundTime =
            processes[selectedProcess].completionTime - processes[selectedProcess].arrivalTime;
        processes[selectedProcess].waitingTime =
            processes[selectedProcess].turnaroundTime - processes[selectedProcess].burstTime;

        isCompleted[selectedProcess] = true;
        completed++;
    }
}

// Function for Priority Preemptive Scheduling
void priorityPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int selectedProcess = -1;

    while (completed < n) {
        int highestPriority = INT_MAX;

        // Select process with the highest priority that has arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0 &&
                processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                selectedProcess = i;
            }
        }

        if (selectedProcess == -1) {
            currentTime++; // Increment time if no process is ready
            continue;
        }

        // Execute the selected process for one unit of time
        processes[selectedProcess].remainingTime--;
        currentTime++;

        // Check if the process is completed
        if (processes[selectedProcess].remainingTime == 0) {
            processes[selectedProcess].completionTime = currentTime;
            processes[selectedProcess].turnaroundTime =
                processes[selectedProcess].completionTime - processes[selectedProcess].arrivalTime;
            processes[selectedProcess].waitingTime =
                processes[selectedProcess].turnaroundTime - processes[selectedProcess].burstTime;
            completed++;
        }
    }
}

// Function to display results
void displayResults(const vector<Process>& processes) {
    cout << "\nProcess ID | Arrival Time | Burst Time | Priority | Completion Time | Turnaround Time | Waiting Time\n";
    cout << "-----------------------------------------------------------------------------------------------\n";

    for (const auto& p : processes) {
        cout << "P" << p.id << "         | "
             << p.arrivalTime << "           | "
             << p.burstTime << "          | "
             << p.priority << "         | "
             << p.completionTime << "             | "
             << p.turnaroundTime << "              | "
             << p.waitingTime << "\n";
    }
}

int main() {
    int n, choice;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
    }

    // Choose scheduling type
    cout << "Choose Scheduling Algorithm:\n";
    cout << "1. Priority Scheduling (Non-Preemptive)\n";
    cout << "2. Priority Scheduling (Preemptive)\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        priorityNonPreemptive(processes);
    } else if (choice == 2) {
        priorityPreemptive(processes);
    } else {
        cout << "Invalid choice!\n";
        return 1;
    }

    // Display results
    displayResults(processes);

    return 0;
}
