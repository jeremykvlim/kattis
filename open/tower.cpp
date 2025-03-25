#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto h = 0LL;
    vector<int> s(n), t(n), all(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> s[i] >> t[i];

        h += s[i] + t[i];
        all[i] = s[i];
        all[i + n] = t[i];
    }
    sort(all.begin(), all.end());
    all.erase(unique(all.begin(), all.end()), all.end());

    for (int i = 0; i < n; i++) {
        s[i] = lower_bound(all.begin(), all.end(), s[i]) - all.begin();
        t[i] = lower_bound(all.begin(), all.end(), t[i]) - all.begin();
    }

    int m = all.size();
    DisjointSets dsu(m);
    vector<bool> same(m, false);
    for (int i = 0; i < n; i++)
        if (!dsu.unite(s[i], t[i])) same[s[i]] = same[t[i]] = true;

    vector<long long> sum(m, 0), longest(m, 0);
    for (int i = 0; i < m; i++) {
        int i_set = dsu.find(i);
        sum[i_set] += all[i];
        longest[i_set] = max(longest[i_set], (long long) all[i]);
        if (same[i]) same[i_set] = true;
    }

    for (int i = 0; i < m; i++) {
        int i_set = dsu.find(i);
        if (i != i_set) continue;
        h -= sum[i_set];
        if (!same[i_set]) h += longest[i_set];
    }

    cout << h;
}
