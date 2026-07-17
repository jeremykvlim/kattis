#include <bits/stdc++.h>
using namespace std;

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

    int n;
    cin >> n;

    vector<int> s(n + 1), p(n + 1), a(n + 1), b(n + 1);
    for (int i = 1; i <= n; i++) cin >> s[i] >> p[i] >> a[i] >> b[i];

    auto score = [&](const auto &risk) {
        vector<double> pr(n + 1, 1), e(n + 1);
        priority_queue<tuple<double, int, int>> pq;
        for (int i = 1; i <= n; i++) {
            pr[i] = (100 - risk[i]) / 100.0L;
            e[i] = s[i] * pr[i];
            pq.emplace(e[i] / (1 - pr[i]), i, 0);
        }

        DisjointSets dsu(n + 1);
        vector<int> count(n + 1, 0), offset(n + 1);
        iota(offset.begin(), offset.end(), 0);
        for (int _ = 0; _ < n; _++) {
            int j = 0;
            while (!pq.empty()) {
                auto [value, i, c] = pq.top();
                pq.pop();

                if (dsu.find(i) == i && i != dsu.find(0) && count[i] == c) {
                    j = i;
                    break;
                }
            }

            int i = dsu.find(p[offset[j]]);
            auto [big, small] = dsu.unite(i, j);
            e[big] = e[i] + pr[i] * e[j];
            pr[big] = pr[i] * pr[j];
            offset[big] = offset[i];
            count[big]++;
            if (big != dsu.find(0)) pq.emplace(e[big] / (1 - pr[big]), big, count[big]);
        }

        return e[dsu.find(0)];
    };
    auto rasmus = score(a), ryan = score(b);

    if (abs(rasmus - ryan) <= 1e-6) cout << "Tie";
    else if (rasmus > ryan) cout << "Rasmus";
    else cout << "Ryan";
}
