#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>
using namespace std;

// ========================== Memory Allocation ==========================
void memoryAllocation() {
    int b, p;
    cout << "\nEnter number of memory blocks: ";
    cin >> b;
    vector<int> blockSize(b);
    cout << "Enter sizes of memory blocks:\n";
    for (int i = 0; i < b; ++i)
        cin >> blockSize[i];

    cout << "Enter number of processes for allocation: ";
    cin >> p;
    vector<int> processSize(p);
    cout << "Enter sizes of processes:\n";
    for (int i = 0; i < p; ++i)
        cin >> processSize[i];

    // First Fit
    vector<int> ff(blockSize);
    cout << "\nFirst Fit Allocation:\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < p; ++i) {
        bool allocated = false;
        for (int j = 0; j < b; ++j) {
            if (ff[j] >= processSize[i]) {
                cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t" << j + 1 << endl;
                ff[j] -= processSize[i];
                allocated = true;
                break;
            }
        }
        if (!allocated)
            cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\tNot Allocated\n";
    }

    // Best Fit
    vector<int> bf(blockSize);
    cout << "\nBest Fit Allocation:\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < p; ++i) {
        int bestIdx = -1;
        for (int j = 0; j < b; ++j) {
            if (bf[j] >= processSize[i]) {
                if (bestIdx == -1 || bf[j] < bf[bestIdx])
                    bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t" << bestIdx + 1 << endl;
            bf[bestIdx] -= processSize[i];
        } else {
            cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\tNot Allocated\n";
        }
    }

    // Worst Fit
    vector<int> wf(blockSize);
    cout << "\nWorst Fit Allocation:\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < p; ++i) {
        int worstIdx = -1;
        for (int j = 0; j < b; ++j) {
            if (wf[j] >= processSize[i]) {
                if (worstIdx == -1 || wf[j] > wf[worstIdx])
                    worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t" << worstIdx + 1 << endl;
            wf[worstIdx] -= processSize[i];
        } else {
            cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\tNot Allocated\n";
        }
    }
}

// ========================== LRU Page Replacement ==========================
void lruPageReplacement() {
    int n, f;
    cout << "\nEnter number of frames for LRU: ";
    cin >> f;
    cout << "Enter number of pages: ";
    cin >> n;
    vector<int> pages(n);
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; ++i) cin >> pages[i];

    vector<int> memory;
    int faults = 0;
    cout << "\nLRU Page Replacement Simulation:\n";
    for (int i = 0; i < n; ++i) {
        auto it = find(memory.begin(), memory.end(), pages[i]);
        if (it == memory.end()) {
            if (memory.size() == f)
                memory.erase(memory.begin());
            memory.push_back(pages[i]);
            faults++;
        } else {
            memory.erase(it);
            memory.push_back(pages[i]);
        }
        cout << "Step " << i + 1 << " (Page " << pages[i] << "): ";
        for (int val : memory) cout << val << " ";
        cout << endl;
    }
    cout << "Total Page Faults: " << faults << "\n";
}

// ========================== FIFO Page Replacement ==========================
void fifoPageReplacement() {
    int n, f;
    cout << "\nEnter number of frames for FIFO: ";
    cin >> f;
    cout << "Enter number of pages: ";
    cin >> n;
    vector<int> pages(n);
    cout << "Enter page reference string:\n";
    for (int i = 0; i < n; ++i) cin >> pages[i];

    queue<int> memory;
    vector<int> frames;
    int faults = 0;

    cout << "\nFIFO Page Replacement Simulation:\n";
    for (int i = 0; i < n; ++i) {
        if (find(frames.begin(), frames.end(), pages[i]) == frames.end()) {
            if (frames.size() == f) {
                int removed = memory.front();
                memory.pop();
                frames.erase(remove(frames.begin(), frames.end(), removed), frames.end());
            }
            memory.push(pages[i]);
            frames.push_back(pages[i]);
            faults++;
        }
        cout << "Step " << i + 1 << " (Page " << pages[i] << "): ";
        for (int val : frames) cout << val << " ";
        cout << endl;
    }
    cout << "Total Page Faults: " << faults << "\n";
}

// ========================== Banker's Algorithm ==========================
bool isSafe(vector<vector<int>> &alloc, vector<vector<int>> &maxNeed, vector<int> &avail, int n, int r) {
    vector<vector<int>> need(n, vector<int>(r));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < r; ++j)
            need[i][j] = maxNeed[i][j] - alloc[i][j];

    vector<bool> finish(n, false);
    vector<int> work = avail;
    vector<int> safeSeq;

    while (safeSeq.size() < n) {
        bool found = false;
        for (int i = 0; i < n; ++i) {
            if (!finish[i]) {
                bool possible = true;
                for (int j = 0; j < r; ++j)
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                if (possible) {
                    for (int j = 0; j < r; ++j)
                        work[j] += alloc[i][j];
                    safeSeq.push_back(i);
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }

    cout << "System is in a SAFE state.\nSafe Sequence: ";
    for (int i : safeSeq) cout << "P" << i << " ";
    cout << "\n";
    return true;
}

void bankersAlgorithm() {
    int n, r;
    cout << "\nEnter number of processes: ";
    cin >> n;
    cout << "Enter number of resources: ";
    cin >> r;

    vector<vector<int>> alloc(n, vector<int>(r)), maxNeed(n, vector<int>(r));
    vector<int> avail(r);

    cout << "Enter Allocation Matrix:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < r; ++j)
            cin >> alloc[i][j];

    cout << "Enter Maximum Need Matrix:\n";
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < r; ++j)
            cin >> maxNeed[i][j];

    cout << "Enter Available Resources:\n";
    for (int i = 0; i < r; ++i)
        cin >> avail[i];

    if (!isSafe(alloc, maxNeed, avail, n, r))
        cout << "System is NOT in a safe state.\n";
}

// ========================== Main ==========================
int main() {
    memoryAllocation();
    lruPageReplacement();
    fifoPageReplacement();
    bankersAlgorithm();
    return 0;
}
