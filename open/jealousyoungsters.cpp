#include <bits/stdc++.h>
using namespace std;

vector<int> gale_shapley(const vector<vector<int>> &pref, const vector<vector<int>> &rank) {
    int n = pref.size(), m = rank.size();
    if (n > m) return {};

    vector<int> next(n, 0), remaining(n), match_p(n, -1), match_r(m, -1);
    iota(remaining.begin(), remaining.end(), 0);

    while (!remaining.empty()) {
        int v = remaining.back();
        remaining.pop_back();

        while (next[v] < pref[v].size()) {
            int u = pref[v][next[v]++];
            if (!~match_r[u]) {
                match_r[u] = v;
                match_p[v] = u;
                break;
            }

            int t = match_r[u];
            if (rank[u][v] < rank[u][t]) {
                match_r[u] = v;
                match_p[v] = u;
                match_p[t] = -1;
                remaining.emplace_back(t);
                break;
            }
        }
    }

    return match_p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, d, e;
    cin >> n >> m >> d >> e;

    if (m < n) {
        cout << "impossible";
        exit(0);
    }

    vector<int> start(n, 0), toy(n, -1);
    vector<vector<int>> pref(n), rank(m, vector<int>(n, 0));
    vector<vector<bool>> seen(n, vector<bool>(m, false));
    while (e--) {
        int s, k, t;
        cin >> s >> k >> t;
        k--;

        if (~toy[k]) rank[toy[k]][k] += s - start[k];

        if (!t) toy[k] = -1;
        else {
            t--;
            if (!seen[k][t]) {
                seen[k][t] = true;
                pref[k].emplace_back(t);
            }
            toy[k] = t;
        }
        start[k] = s;
    }

    for (int k = 0; k < n; k++) {
        if (~toy[k]) rank[toy[k]][k] += d - start[k];
        for (int t = 0; t < m; t++)
            if (!seen[k][t]) pref[k].emplace_back(t);
    }

    for (int i : gale_shapley(pref, rank)) cout << i + 1 << " ";
}
