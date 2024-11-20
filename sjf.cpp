#include <bits/stdc++.h>
using namespace std;

// Structure to represent a process
struct Process {
    int id;           // Process ID
    int arrivalTime;  // Arrival time of the process
    int burstTime;    // Burst time of the process
    int remainingTime; // Remaining burst time (used in preemptive scheduling)
    int completionTime; // Time when process finishes execution
    int turnaroundTime; // Turnaround time (TAT = Completion Time - Arrival Time)
    int waitingTime;    // Waiting time (WT = Turnaround Time - Burst Time)
};

// Function to calculate TAT and WT using SJF Non-Preemptive
void calculateSJFNonPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    vector<bool> isCompleted(n, false);

    while (completed < n) {
        int minBurstTime = INT_MAX, selectedProcess = -1;

        // Select the process with the shortest burst time among those that have arrived
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrivalTime <= currentTime &&
                processes[i].burstTime < minBurstTime) {
                minBurstTime = processes[i].burstTime;
                selectedProcess = i;
            }
        }

        if (selectedProcess == -1) {
            currentTime++; // Increment time if no process is ready
            continue;
        }

        // Update completion time and mark the process as completed
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

// Function to calculate TAT and WT using SJF Preemptive
void calculateSJFPreemptive(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int minRemainingTime = INT_MAX, selectedProcess = -1;

    while (completed < n) {
        // Select the process with the shortest remaining burst time among those that have arrived
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0 &&
                processes[i].remainingTime < minRemainingTime) {
                minRemainingTime = processes[i].remainingTime;
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
            minRemainingTime = INT_MAX; // Reset minimum remaining time
        }
    }
}

// Function to display the results
void displayResults(const vector<Process>& processes) {
    cout << "\nProcess ID | Arrival Time | Burst Time | Completion Time | Turnaround Time | Waiting Time\n";
    cout << "----------------------------------------------------------------------------------------\n";

    for (const auto& p : processes) {
        cout << "P" << p.id << "         | "
             << p.arrivalTime << "           | "
             << p.burstTime << "          | "
             << p.completionTime << "             | "
             << p.turnaroundTime << "            | "
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
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
    }

    // Choose scheduling type
    cout << "Choose Scheduling Algorithm:\n";
    cout << "1. Shortest Job First (Non-Preemptive)\n";
    cout << "2. Shortest Job First (Preemptive)\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        calculateSJFNonPreemptive(processes);
    } else if (choice == 2) {
        calculateSJFPreemptive(processes);
    } else {
        cout << "Invalid choice!\n";
        return 1;
    }

    // Display the results
    displayResults(processes);

    return 0;
}
