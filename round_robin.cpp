#include <bits/stdc++.h>
using namespace std;

struct Process {
    int pid;              // Process ID
    int arrivalTime;      // Arrival time
    int burstTime;        // Burst time
    int remainingTime;    // Remaining time for execution
    int completionTime;   // Completion time
    int turnaroundTime;   // Turnaround time
    int waitingTime;      // Waiting time
};

void roundRobin(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    int currentTime = 0;
    queue<int> readyQueue; // Queue to store process indices
    vector<bool> inQueue(n, false); // Track whether a process is in the queue
    int completed = 0; // Number of completed processes

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    });

    // Start with the first process
    readyQueue.push(0);
    inQueue[0] = true;

    while (completed < n) {
        if (!readyQueue.empty()) {
            int index = readyQueue.front();
            readyQueue.pop();
            inQueue[index] = false;

            // Execute the process for at most timeQuantum units
            int execTime = min(timeQuantum, processes[index].remainingTime);
            currentTime += execTime;
            processes[index].remainingTime -= execTime;

            // Check if the process is completed
            if (processes[index].remainingTime == 0) {
                processes[index].completionTime = currentTime;
                processes[index].turnaroundTime = currentTime - processes[index].arrivalTime;
                processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
                completed++;
            }

            // Add all processes that have arrived by the current time to the queue
            for (int i = 0; i < n; i++) {
                if (i != index && processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && !inQueue[i]) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }

            // If the current process is not yet completed, re-add it to the queue
            if (processes[index].remainingTime > 0) {
                readyQueue.push(index);
                inQueue[index] = true;
            }

            // Handle idle time (no process in the queue)
        } else {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && !inQueue[i]) {
                    readyQueue.push(i);
                    inQueue[i] = true;
                }
            }
        }
    }

    // Display the results
    cout << "\nPID\tArrival\tBurst\tCompletion\tTAT\tWT\n";
    for (const auto& process : processes) {
        cout << "P" << process.pid << "\t" << process.arrivalTime << "\t" << process.burstTime << "\t"
             << process.completionTime << "\t\t" << process.turnaroundTime << "\t" << process.waitingTime << "\n";
    }
}

int main() {
    int n, timeQuantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter arrival time and burst time for Process " << i + 1 << ": ";
        processes[i].pid = i + 1;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
    }

    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    roundRobin(processes, timeQuantum);

    return 0;
}
