#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t h(uint64_t key) {
        uint64_t hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const vector<int> &v) const {
        uint64_t key = 0;
        for (int i : v) key ^= i + 0x9e3779b9 + (key << 6) + (key >> 2);
        return h(key);
    }
};

void identify(set<vector<int>> &states, unordered_set<vector<int>, Hash> &visited, vector<int> curr, vector<int> groups, int mask, int n, int pos = 0) {
    sort(groups.begin(), groups.end());
    if (visited.count(groups)) return;

    visited.emplace(groups);
    if (pos == curr.size()) {
        if (mask & 1) {
            if (groups.size() == n) {
                cout << n;
                exit(0);
            }
            states.emplace(groups);
        }
        return;
    }

    for (int i = 0; i <= mask && i <= curr[pos] / 2; i++) {
        if (i) groups.emplace_back(i);
        if (i != curr[pos]) groups.emplace_back(curr[pos] - i);
        identify(states, visited, curr, groups, (mask >> (curr[pos] - i)) | (mask >> i), n, pos + 1);
        if (i != curr[pos]) groups.pop_back();
        if (i) groups.pop_back();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    set<vector<int>> states{{n}};
    while (m--) {
        int a;
        cin >> a;

        set<vector<int>> temp;
        for (auto &groups : states) {
            unordered_set<vector<int>, Hash> visited;
            identify(temp, visited, groups, {}, 1 << a, n);
        }
        states = temp;
    }

    int colleagues = 0;
    for (auto &groups : states) colleagues = max(colleagues, (int) count_if(groups.begin(), groups.end(), [](auto size) {return size == 1;}));

    cout << colleagues;
}
