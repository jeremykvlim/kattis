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

    int n, k;
    cin >> n >> k;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;

    int v = n + 1;
    for (int p2 = 1; p2 < n; p2 <<= 1) {
        int q = n / p2, r = n % p2;
        if (q == 1) v += r;
        else if (q & 1) v += (n - r + p2) / 2;
        else v += (n + r) / 2;
    }

    int u = 2 * n;
    BoundedFlowNetwork<int, long long> bfn(v + 2);
    auto dnc = [&](auto &&self, int l, int r) -> vector<int> {
        if (l + 1 == r) return {l};

        int m = l + (r - l) / 2;
        auto left = self(self, l, m), right = self(self, m, r);

        int R = right.size();
        for (int i = 0; i + 1 < R; i++) {
            bfn.add_arc(i + u, 2 * right[i], 0, 4, right[i] - m);
            if (i + 2 < R) bfn.add_arc(i + u, i + u + 1, 0, 4);
            else bfn.add_arc(i + u, 2 * right[i + 1], 0, 4, right[i + 1] - m);
        }

        for (int i = 0; int j : left) {
            for (; i < R && h[right[i]] < h[j] + k; i++);
            if (i + 1 < R) bfn.add_arc(2 * j + 1, u + i, 0, 4, m - j - 1);
            else if (i < R) bfn.add_arc(2 * j + 1, 2 * right[i], 0, 4, right[i] - j - 1);
        }
        u += R - 1;

        vector<int> order;
        merge(left.begin(), left.end(), right.begin(), right.end(), back_inserter(order), [&](int i, int j) { return h[i] < h[j]; });
        return order;
    };
    dnc(dnc, 0, n);

    for (int i = 0; i < n; i++) {
        bfn.add_arc(v, 2 * i, 0, 1, i);
        bfn.add_arc(2 * i, 2 * i + 1, 0, 1);
        bfn.add_arc(2 * i + 1, v + 1, 0, 1, n - i - 1);
    }
    bfn.add_arc(v, v + 1, 0, 4, n);
    bfn.add_supply(v, 4);
    bfn.add_demand(v + 1, 4);
    cout << 4 * n - bfn.min_cost_b_flow().first;
}