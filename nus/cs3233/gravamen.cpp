#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, rev;
        T cap;
        Arc(int u, int rev, T cap) : u(u), rev(rev), cap(cap) {}
    };

    int n;
    vector<vector<Arc>> network;
    FlowNetwork(int n) : n(n), network(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    T max_flow(int s, int t) {
        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();
        vector<stack<int>> active_stacks(2 * n);

        auto push = [&](int v, Arc &a, T delta) {
            int u = a.u;
            if (!excess[u] && delta) active_stacks[height[u]].emplace(u);
            a.cap -= delta;
            network[u][a.rev].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        auto relabel = [&](int v, int h) {
            if (h < height[v] && height[v] < n) {
                height[v] = n + 1;
                count[height[v]]--;
            }
        };

        for (auto &&a : network[s]) push(s, a, a.cap);

        auto discharge = [&](int v, int &h) {
            while (excess[v] > 0)
                if (curr[v] == network[v].end()) {
                    height[v] = INT_MAX;

                    for (auto a = network[v].begin(); a != network[v].end(); a++)
                        if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                    count[height[v]]++;
                    if (!--count[h] && h < n)
                        for (int u = 0; u < n; u++) relabel(u, h);
                    h = height[v];
                } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                else curr[v]++;

            while (h >= 0 && active_stacks[h].empty()) h--;
        };

        if (!active_stacks[0].empty())
            for (int h = 0; h >= 0;) {
                int v = active_stacks[h].top();
                active_stacks[h].pop();

                discharge(v, h);
            }

        return -excess[s];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, s, L;
    cin >> R >> s >> L;

    vector<pair<int, int>> edges(L);
    FlowNetwork<int> fn(L + R + s + 2);
    for (int i = 0; i < L; i++) {
        int a, b;
        cin >> a >> b;

        edges[i] = {a, b};
        fn.add_arc(0, i + 1, 1);
        fn.add_arc(i + 1, L + a, 1);
        fn.add_arc(i + 1, L + R + b, 1);
    }

    auto temp = fn;
    int l = 0, r = L, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        for (int i = 0; i < R; i++) fn.add_arc(L + i + 1, L + R + s + 1, m);
        for (int i = 0; i < s; i++) fn.add_arc(L + R + i + 1, L + R + s + 1, m);

        if (fn.max_flow(0, L + R + s + 1) == L) r = m;
        else l = m;

        fn = temp;
    }
    for (int i = 0; i < R; i++) fn.add_arc(L + i + 1, L + R + s + 1, r);
    for (int i = 0; i < s; i++) fn.add_arc(L + R + i + 1, L + R + s + 1, r);
    fn.max_flow(0, L + R + s + 1);

    vector<pair<string, int>> rulings(L);
    for (int i = 0; i < L; i++)
        for (auto [u, v, cap] : fn.network[i + 1])
            if (L < u && u <= L + R + s && !cap) {
                if (u <= L + R) rulings[i] = {"INDV", edges[i].first};
                else rulings[i] = {"CORP", edges[i].second};
                break;
            }

    for (auto [party, i] : rulings) cout << party << " " << i << "\n";
}
