#include <iostream>
#include <vector>

using namespace std;

void calculateNeed(vector<vector<int>> &need, vector<vector<int>> &max, vector<vector<int>> &allocation, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(vector<int> &processes, vector<int> &avail, vector<vector<int>> &max, vector<vector<int>> &allocation, int n, int m) {
    vector<vector<int>> need(n, vector<int>(m));
    calculateNeed(need, max, allocation, n, m);

    vector<bool> finish(n, false);
    vector<int> safeSequence;
    vector<int> work(avail);

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[p][j] > work[j])
                        break;
                }
                if (j == m) {
                    for (int k = 0; k < m; k++)
                        work[k] += allocation[p][k];
                    safeSequence.push_back(processes[p]);
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) {
            cout << "System is not in a safe state." << endl;
            return false;
        }
    }

    cout << "System is in a safe state.\nSafe sequence is: ";
    for (int i = 0; i < n; i++)
        cout << "P" << safeSequence[i] << " ";
    cout << endl;

    return true;
}

int main() {
    int n, m;
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the number of resources: ";
    cin >> m;

    vector<int> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i] = i;
    }

    vector<vector<int>> allocation(n, vector<int>(m));
    cout << "Enter the allocation matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> allocation[i][j];
        }
    }

    vector<vector<int>> max(n, vector<int>(m));
    cout << "Enter the max matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> max[i][j];
        }
    }

    vector<int> avail(m);
    cout << "Enter the available resources:" << endl;
    for (int i = 0; i < m; i++) {
        cin >> avail[i];
    }

    isSafe(processes, avail, max, allocation, n, m);

    return 0;
}
