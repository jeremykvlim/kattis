#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;
        pi--;
    }

    int count = 0;
    bool even = false;
    vector<bool> visited(n, false);
    for (int i = 0; i < n; i++)
        if (!visited[i]) {
            int size = 0;
            for (int j = i; !visited[j]; j = p[j], size++) visited[j] = true;
            if (size == 2) count++;
            else if (!(size & 1)) even = true;
        }

    if (count == 1 && !even) {
        cout << "NO";
        exit(0);
    }

    int components = 0;
    vector<vector<int>> component(n, vector<int>(n, -1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (!~component[i][j]) {
                for (int r = i, c = j; component[r][c] == -1; r = exchange(c, p[r])) component[r][c] = components;
                components++;
            }

    mt19937_64 rng(random_device{}());
    for (;;) {
        vector<int> bit(components);
        for (int &b : bit) b = rng() & 1;

        vector<string> grid(n, string(n, '0'));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (bit[component[i][j]]) grid[i][j] = '1';

        auto sorted = grid;
        sort(sorted.begin(), sorted.end());
        if (adjacent_find(sorted.begin(), sorted.end()) != sorted.end()) goto next;

        cout << "YES\n";
        for (auto &row : grid) cout << row << "\n";
        exit(0);
        next:;
    }
}
