#include <bits/stdc++.h>
using namespace std;

long long maintain(int v, int mod, vector<unordered_map<int, int>> &build, vector<long long> &m) {
    if (m[v] != -1) return m[v];

    auto c = 1LL;
    for (auto [u, count] : build[v]) c += (maintain(u, mod, build, m) * count) % mod;

    return m[v] = c % mod;
}

long long rebuild(int v, int mod, unordered_set<int> &y, vector<unordered_map<int, int>> &build, vector<long long> &m, vector<pair<long long, bool>> &r) {
    if (r[v].first != -1) return r[v].first;

    if (y.count(v)) {
        r[v].second = true;
        return maintain(v, mod, build, m) % mod;
    }

    auto c = 0LL;
    bool extra = false;
    for (auto [u, count] : build[v]) {
        c += (rebuild(u, mod, y, build, m, r) * count) % mod;
        extra |= r[u].second;
    }

    return (r[v] = {(c + extra) % mod, extra}).first % mod;
}

bool dfs(int v, vector<unordered_map<int, int>> &adj_list, vector<char> &state, vector<unordered_map<int, int>> &build) {
    state[v] = 1;
    build[v] = adj_list[v];
    for (auto [u, count] : adj_list[v])
        if (!state[u]) {
            if (dfs(u, adj_list, state, build)) return true;
        }
        else if (state[u] == 1) return true;
    state[v] = 2;

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, mod;
    cin >> n >> mod;

    vector<unordered_map<int, int>> adj_list(n);
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;

        while (c--) {
            int d;
            cin >> d;

            adj_list[i][d]++;
        }
    }
    vector<char> state(n, 0);
    vector<unordered_map<int, int>> build(n);
    if (dfs(0, adj_list, state, build) || build[0].empty()) {
        cout << "Invalid";
        exit(0);
    }

    int q, t;
    cin >> q >> t;

    if (t) {
        cout << "Valid";
        exit(0);
    }

    vector<long long> m(n, -1);
    cout << maintain(0, mod, build, m) << "\n";

    while (q--) {
        int x;
        cin >> x;

        unordered_set<int> y;
        while (x--) {
            int yi;
            cin >> yi;

            y.emplace(yi);
        }

        vector<pair<long long, bool>> r(n, {-1, false});
        cout << rebuild(0, mod, y, build, m, r) << "\n";
    }
}
