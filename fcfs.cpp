#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>  // for setting precision

using namespace std;

// Structure to represent a process
struct Process {
    int id;             // Process ID
    int arrivalTime;    // Arrival time of the process
    int burstTime;      // CPU Burst time of the process
    int completionTime; // Time when process finishes execution
    int turnaroundTime; // Turnaround time (TAT = Completion Time - Arrival Time)
    int waitingTime;    // Waiting time (WT = Turnaround Time - Burst Time)
};

void calculateFCFS(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;

    // Calculate Completion Time for each process
    for (int i = 0; i < n; i++) {
        // Set start time to the maximum of current time and arrival time
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;

        // Completion time is current time + burst time
        processes[i].completionTime = currentTime + processes[i].burstTime;
        // Update current time for the next process
        currentTime = processes[i].completionTime;

        // Calculate Turnaround Time (TAT = Completion Time - Arrival Time)
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
        // Calculate Waiting Time (WT = Turnaround Time - Burst Time)
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
    }
}

void displayResults(const vector<Process>& processes) {
    cout << fixed << setprecision(2);
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
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    // Calculate TAT and WT using FCFS Scheduling
    calculateFCFS(processes);

    // Display the results
    displayResults(processes);

    return 0;
}
