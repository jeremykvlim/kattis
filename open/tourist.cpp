#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
struct BoundedFlowNetwork {
    struct Arc {
        int u, v;
        T cap;
        U cost;
        Arc(int u, int v, T cap, U cost) : u(u), v(v), cap(cap), cost(cost) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<T> balance;
    U cost;

    BoundedFlowNetwork(int n) : n(n), network(n + 2), balance(n + 2, 0), cost(0) {}

    void add_supply(int v, T b) {
        balance[v] += b;
    }

    void add_demand(int v, T b) {
        balance[v] -= b;
    }

    pair<int, int> add_arc(int u, int v, T lb, T ub, U c = 0) {
        int su = network[u].size(), sv = network[v].size();
        T f = c < 0 ? ub : lb;
        cost += c * f;
        add_supply(v, f);
        add_demand(u, f);
        if (u == v) return {su, sv};

        if (c < 0) {
            network[v].emplace_back(u, network[u].size(), ub - lb, -c);
            network[u].emplace_back(v, network[v].size() - 1, 0, c);
        } else {
            network[u].emplace_back(v, network[v].size(), ub - lb, c);
            network[v].emplace_back(u, network[u].size() - 1, 0, -c);
        }
        return {su, sv};
    }

    void successive_shortest_path() {
        for (int v = 0; v < n; v++)
            if (balance[v] > 0) add_arc(n, v, 0, balance[v]);
            else if (balance[v] < 0) add_arc(v, n + 1, 0, -balance[v]);

        U inf = numeric_limits<U>::max();
        vector<U> potential(n + 2, 0), dist(n + 2);
        vector<pair<int, int>> prev(n + 2, {-1, -1});
        priority_queue<pair<U, int>, vector<pair<U, int>>, greater<>> pq;

        for (;;) {
            fill(dist.begin(), dist.end(), inf);
            dist[n] = 0;
            pq.emplace(0, n);

            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();

                if (dist[v] != d) continue;

                for (int i = 0; i < network[v].size(); i++) {
                    auto &[u, _, cap, c] = network[v][i];
                    U phi = c + potential[v] - potential[u];
                    if (cap > 0 && dist[u] > d + phi) {
                        dist[u] = d + phi;
                        pq.emplace(d + phi, u);
                        prev[u] = {v, i};
                    }
                }
            }

            if (dist[n + 1] == inf) break;

            for (int v = 0; v < n + 2; v++)
                if (dist[v] != inf) potential[v] += dist[v];

            T f = numeric_limits<T>::max();
            for (int v = n + 1; v != n; v = prev[v].first) {
                auto [u, e] = prev[v];
                f = min(f, network[u][e].cap);
            }

            cost += (potential[n + 1] - potential[n]) * f;
            for (int v = n + 1; v != n; v = prev[v].first) {
                auto [u, e] = prev[v];
                auto &[w, rev, cap, c] = network[u][e];
                cap -= f;
                network[v][rev].cap += f;
            }
        }
    }

    bool feasible() {
        if (accumulate(balance.begin(), balance.end(), (T) 0)) return false;
        return all_of(network[n].begin(), network[n].end(), [](auto &a) { return !a.cap; });
    }

    tuple<T, U, bool> min_cost_max_flow(int s, int t) {
        U penalty = 1;
        for (int v = 0; v < n; v++)
            for (auto &a : network[v])
                if (a.cost > 0) penalty += a.cost;

        T cap = -balance[s];
        for (auto &a : network[s]) cap += a.cap;

        auto [st, ss] = add_arc(t, s, 0, max(cap, (T) 0), -penalty);
        successive_shortest_path();
        if (!feasible()) return {0, 0, false};
        return {network[s][ss].cap, cost + penalty * network[s][ss].cap, true};
    }

    pair<U, bool> min_cost_b_flow() {
        successive_shortest_path();
        if (!feasible()) return {0, false};
        return {cost, true};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    while (tc--) {
        int w, h;
        cin >> w >> h;

        vector<string> city(h);
        for (auto &row : city) cin >> row;

        auto index = [&](int i, int j) {
            return i * w + j;
        };

        BoundedFlowNetwork<int, int> bfn(2 * w * h);
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                if (city[i][j] != '#') {
                    if (city[i][j] == '*') {
                        bfn.add_arc(2 * index(i, j), 2 * index(i, j) + 1, 0, 1, 0);
                        bfn.add_arc(2 * index(i, j), 2 * index(i, j) + 1, 0, 1, 1);
                    } else bfn.add_arc(2 * index(i, j), 2 * index(i, j) + 1, 0, 2, 1);
                    
                    if (i + 1 < h && city[i + 1][j] != '#') bfn.add_arc(2 * index(i, j) + 1, 2 * index(i + 1, j), 0, 2);
                    if (j + 1 < w && city[i][j + 1] != '#') bfn.add_arc(2 * index(i, j) + 1, 2 * index(i, j + 1), 0, 2);
                }
        bfn.add_supply(0, 2);
        bfn.add_demand(2 * index(h - 1, w - 1) + 1, 2);
        cout << 2 * (w + h - 1) - bfn.min_cost_b_flow().first << "\n";
    }
}
