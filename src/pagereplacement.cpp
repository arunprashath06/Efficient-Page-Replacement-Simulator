#include <iostream>
#include <climits>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <iomanip>
#include <string>
using namespace std;

struct Result {
    vector<vector<int>> frames_over_time;
    vector<bool> fault;
    int faults;
};

vector<int> parse_refstring(const string &s) {
    vector<int> v; stringstream ss(s); int x;
    while (ss >> x) v.push_back(x);
    return v;
}

Result simulate_fifo(const vector<int>& ref, int frames) {
    Result r;
    r.fault.reserve(ref.size());
    r.frames_over_time.assign(frames, vector<int>(ref.size(), -1));
    queue<int> q;
    unordered_set<int> inmem;
    vector<int> cur(frames, -1);
    int faults = 0;

    for (size_t i = 0; i < ref.size(); ++i) {
        int p = ref[i];
        if (inmem.find(p) == inmem.end()) {
            faults++;
            if ((int)q.size() < frames) {
                q.push(p);
                inmem.insert(p);
                for (int f = 0; f < frames; f++)
                    if (cur[f] == -1) { cur[f] = p; break; }
            } else {
                int out = q.front(); q.pop();
                inmem.erase(out);
                q.push(p);
                inmem.insert(p);
                for (int f = 0; f < frames; f++)
                    if (cur[f] == out) { cur[f] = p; break; }
            }
            r.fault.push_back(true);
        } else r.fault.push_back(false);

        for (int f = 0; f < frames; f++)
            r.frames_over_time[f][i] = cur[f];
    }

    r.faults = faults;
    return r;
}

Result simulate_lru(const vector<int>& ref, int frames) {
    Result r;
    r.fault.reserve(ref.size());
    r.frames_over_time.assign(frames, vector<int>(ref.size(), -1));

    vector<int> cur(frames, -1);
    unordered_map<int, int> pos;
    int faults = 0;

    for (size_t i = 0; i < ref.size(); ++i) {
        int p = ref[i];

        if (pos.find(p) != pos.end()) {
            pos[p] = i;
            r.fault.push_back(false);
        } else {
            faults++;
            bool placed = false;

            for (int f = 0; f < frames; f++)
                if (cur[f] == -1) {
                    cur[f] = p;
                    pos[p] = i;
                    placed = true;
                    break;
                }

            if (!placed) {
                int lru_idx = -1, lru_time = INT_MAX;
                for (int f = 0; f < frames; f++) {
                    int val = cur[f];
                    int last = pos[val];
                    if (last < lru_time) {
                        lru_time = last;
                        lru_idx = f;
                    }
                }
                pos.erase(cur[lru_idx]);
                cur[lru_idx] = p;
                pos[p] = i;
            }
            r.fault.push_back(true);
        }

        for (int f = 0; f < frames; f++)
            r.frames_over_time[f][i] = cur[f];
    }

    r.faults = faults;
    return r;
}

Result simulate_optimal(const vector<int>& ref, int frames) {
    Result r;
    r.fault.reserve(ref.size());
    r.frames_over_time.assign(frames, vector<int>(ref.size(), -1));
    vector<int> cur(frames, -1);
    int faults = 0;

    for (size_t i = 0; i < ref.size(); ++i) {
        int p = ref[i];
        bool hit = false;

        for (int f = 0; f < frames; f++)
            if (cur[f] == p) { hit = true; break; }

        if (hit) {
            r.fault.push_back(false);
        } else {
            faults++;
            bool placed = false;

            for (int f = 0; f < frames; f++)
                if (cur[f] == -1) {
                    cur[f] = p;
                    placed = true;
                    break;
                }

            if (!placed) {
                int victim = -1, farthest = -1;

                for (int f = 0; f < frames; f++) {
                    int val = cur[f];
                    int j;
                    for (j = i + 1; j < (int)ref.size(); j++)
                        if (ref[j] == val) break;

                    if (j == (int)ref.size()) {
                        victim = f;
                        break;
                    }
                    if (j > farthest) {
                        farthest = j;
                        victim = f;
                    }
                }

                cur[victim] = p;
            }

            r.fault.push_back(true);
        }

        for (int f = 0; f < frames; f++)
            r.frames_over_time[f][i] = cur[f];
    }

    r.faults = faults;
    return r;
}

void print_table(const vector<int>& ref, const Result& res) {
    int frames = res.frames_over_time.size();

    cout << "\nReference string:\n";
    for (int x : ref) cout << x << " ";
    cout << "\n\nFrame evolution:\n";

    cout << "Time\\Frame\t";
    for (size_t i = 0; i < ref.size(); ++i) cout << i << "\t";
    cout << "\n";

    for (int f = 0; f < frames; f++) {
        cout << "Frame " << f << "\t";
        for (size_t t = 0; t < ref.size(); t++) {
            int v = res.frames_over_time[f][t];
            if (v == -1) cout << "-\t";
            else cout << v << "\t";
        }
        cout << "\n";
    }

    cout << "Fault\t\t";
    for (bool b : res.fault) cout << (b ? "F\t" : " \t");
    cout << "\n\nTotal Requests: " << ref.size()
         << "\nPage Faults: " << res.faults
         << "\nHit Ratio: " << fixed << setprecision(2)
         << (100.0 * (ref.size() - res.faults) / ref.size()) << "%\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "Efficient Page Replacement Algorithm Simulator\n";
    cout << "Enter reference string (space-separated integers):\n";

    string line;
    getline(cin, line);
    while (line.size() == 0) getline(cin, line);

    vector<int> ref = parse_refstring(line);

    cout << "Enter number of frames:\n";
    int frames;
    while (!(cin >> frames) || frames <= 0) {
        cin.clear();
        string dummy;
        getline(cin, dummy);
        cout << "Enter valid positive integer:\n";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (true) {
        cout << "\nMenu:\n1. FIFO\n2. LRU\n3. Optimal\n4. Compare All\n5. Exit\nChoose option: ";

        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            string d; getline(cin, d);
            continue;
        }

        if (opt == 5) break;

        if (opt == 1) {
            Result r = simulate_fifo(ref, frames);
            print_table(ref, r);
        }
        else if (opt == 2) {
            Result r = simulate_lru(ref, frames);
            print_table(ref, r);
        }
        else if (opt == 3) {
            Result r = simulate_optimal(ref, frames);
            print_table(ref, r);
        }
        else if (opt == 4) {
            Result r1 = simulate_fifo(ref, frames);
            Result r2 = simulate_lru(ref, frames);
            Result r3 = simulate_optimal(ref, frames);

            cout << "\nComparison:\n";
            cout << "Algorithm\tFaults\tHit%\n";

            cout << "FIFO\t\t" << r1.faults << "\t"
                 << fixed << setprecision(2)
                 << (100.0 * (ref.size() - r1.faults) / ref.size()) << "%\n";

            cout << "LRU\t\t" << r2.faults << "\t"
                 << fixed << setprecision(2)
                 << (100.0 * (ref.size() - r2.faults) / ref.size()) << "%\n";

            cout << "Optimal\t\t" << r3.faults << "\t"
                 << fixed << setprecision(2)
                 << (100.0 * (ref.size() - r3.faults) / ref.size()) << "%\n";
        }
        else cout << "Invalid option\n";
    }

    cout << "Exiting simulator.\n";
    return 0;
}
