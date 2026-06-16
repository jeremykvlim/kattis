#include <bits/stdc++.h>
using namespace std;

vector<int> z_function(const string &s) {
    int n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    auto rev = s;
    reverse(rev.begin(), rev.end());

    auto both = s + '#' + rev;
    auto z_s = z_function(s), z_both = z_function(both);

    vector<vector<int>> adj_list_s(n + 2), adj_list_rev(n + 2);
    for (int i = 0; i < n; i++) {
        adj_list_s[z_s[i] + 1].emplace_back(i);
        adj_list_rev[z_both[2 * n - i] + 1].emplace_back(i);
    }

    DisjointSets dsu_s(n + 1), dsu_rev(n + 1);
    vector<int> rep_s(n + 1), rep_rev(n + 1);
    iota(rep_s.begin(), rep_s.end(), 0);
    iota(rep_rev.begin(), rep_rev.end(), 0);
    vector<bool> merged_s(n, false), merged_rev(n, false);

    auto merge = [&](int i, auto &dsu, auto &merged, auto &rep) {
        if (merged[i]) return;
        merged[i] = true;

        auto [big, small] = dsu.unite(i, i + 1);
        if (small != -1) rep[big] = min(rep[big], rep[small]);
    };

    for (int len = 1, l = 0, r = n - 1; len <= n; len++) {
        for (int i : adj_list_s[len]) merge(i, dsu_s, merged_s, rep_s);
        for (int i : adj_list_rev[len]) merge(i, dsu_rev, merged_rev, rep_rev);

        for (; l < len - 1; l++) merge(l, dsu_rev, merged_rev, rep_rev);
        for (; r > n - len; r--) merge(r, dsu_s, merged_s, rep_s);

        for (int k = len - 1; k < n - 1;) {
            int i = rep_s[dsu_s.find(k + 2)] - 1, j = rep_rev[dsu_rev.find(min(k + len + 1, n))] - 1, m = max(i >= 0 ? i + len - 1 : -1, j);
            if (k >= m) goto next;
            k = m;
        }

        cout << len << " ";
        next:;
    }
}