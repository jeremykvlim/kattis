#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> junctions(n - 1);
    for (auto &j : junctions) {
        cin >> j.first >> j.second;
        if (j.first > j.second) swap(j.first, j.second);
    }
    sort(junctions.begin(), junctions.begin() + n - 1);
    junctions.erase(unique(junctions.begin(), junctions.end()), junctions.end());
    
    vector<int> count(n + 1, 0), prev(n + 1);
    for (auto &j : junctions) {
        count[j.first]++;
        count[j.second]++;
    }

    vector<vector<int>> connected(n + 1);
    for (auto &j : junctions) {
        if (count[j.first] != count[j.second] ? count[j.first] > count[j.second] : j.first < j.second) connected[j.first].emplace_back(j.second);
        else connected[j.second].emplace_back(j.first);
    }

    long long total = 0, optimal = 0;
    for (int i = 1; i <= n; i++) {
        if (count[i]) total += (long long) count[i] * (count[i] - 1);
        for (int j : connected[i]) prev[j] = i;
        for (int j : connected[i])
            for (int k : connected[j])
                if (prev[k] == i) optimal++;
    }

    cout << total - 6 * optimal;
}
