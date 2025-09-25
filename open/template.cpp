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

struct ForwardStar {
    vector<int> head, next;

    ForwardStar() {}
    ForwardStar(int n, int m) : head(n, -1), next(m) {}

    void extend() {
        head.emplace_back(-1);
    }

    void add_edge(int u, int edge_id) {
        if (next.size() < edge_id + 1) next.resize(edge_id + 1);
        next[edge_id] = head[u];
        head[u] = edge_id;
    }
};

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

    string s;
    cin >> s;
    int n = s.size();

    auto rev = s;
    reverse(rev.begin(), rev.end());

    auto both = s + '#' + rev;
    auto z_s = z_function(s), z_both = z_function(both);

    ForwardStar list_s(n + 2, n), list_rev(n + 2, n);
    for (int i = 0; i < n; i++) {
        list_s.add_edge(z_s[i] + 1, i);
        list_rev.add_edge(z_both[2 * n - i] + 1, i);
    }

    DisjointSets dsu_s(n + 1), dsu_rev(n + 1);
    vector<bool> merged_s(n, false), merged_rev(n, false);

    auto merge = [&](int i, auto &dsu, auto &merged) {
        if (merged[i]) return;
        merged[i] = true;
        dsu.unite(i, i + 1);
    };

    for (int len = 1, l = 0, r = n - 1; len <= n; len++) {
        for (int i = list_s.head[len]; i != -1; i = list_s.next[i]) merge(i, dsu_s, merged_s);
        for (int i = list_rev.head[len]; i != -1; i = list_rev.next[i]) merge(i, dsu_rev, merged_rev);

        for (; l < len - 1; l++) merge(l, dsu_rev, merged_rev);
        for (; r > n - len; r--) merge(r, dsu_s, merged_s);

        for (int k = len - 1; k < n - 1;) {
            int i = dsu_s.find(k + 2) - 1, j = dsu_rev.find(min(k + len + 1, n)) - 1, m = max(i >= 0 ? i + len - 1 : -1, j);
            if (k >= m) goto next;
            k = m;
        }
        cout << len << " ";
        next:;
    }
}
