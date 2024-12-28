#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, v;
        T cap, cost;
        Arc(int u, int v, T cap, T cost) : u(u), v(v), cap(cap), cost(cost) {}
    };

    int n;
    vector<vector<Arc>> network;
    T inf;
    FlowNetwork(int n) : n(n), network(n), inf(numeric_limits<T>::max()) {}

    void add_arc(int u, int v, T cap_uv, T cost, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv, cost);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu, -cost);
    }

    T max_flow(int s, int t) {
        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), height_count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        height_count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();

        vector<vector<int>> active(2 * n);
        auto push = [&](int v, Arc &a, T delta) {
            int u = a.u;
            if (!abs(excess[u]) && delta > 0) active[height[u]].emplace_back(u);
            a.cap -= delta;
            network[u][a.v].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        for (auto &&a : network[s]) push(s, a, a.cap);

        if (!active[0].empty())
            for (int h = 0; h >= 0;) {
                int v = active[h].back();
                active[h].pop_back();

                while (excess[v] > 0)
                    if (curr[v] == network[v].end()) {
                        height[v] = INT_MAX;

                        for (auto a = network[v].begin(); a != network[v].end(); a++)
                            if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                        height_count[height[v]]++;
                        if (!--height_count[h] && h < n)
                            for (int w = 0; w < n; w++)
                                if (h < height[w] && height[w] < n) {
                                    height_count[height[w]]--;
                                    height[w] = n + 1;
                                }
                        h = height[v];
                    } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                    else curr[v]++;

                while (h >= 0 && active[h].empty()) h--;
            }

        return -excess[s];
    }

    pair<T, T> max_flow_min_cost(int s, int t) {
        T cost = 0, bound = 0, shift = min(3, __lg(n));
        for (int v = 0; v < n; v++)
            for (auto &&a : network[v]) {
                cost += a.cost * a.cap;
                a.cost <<= shift;
                bound = max(bound, a.cost);
            }

        T flow = max_flow(s, t);

        vector<T> phi(n, 0), excess(n, 0);
        vector<int> count(n, 0);
        deque<int> st;

        auto push = [&](int v, Arc &a, T delta, bool active) {
            delta = min(delta, a.cap);
            int u = a.u;
            a.cap -= delta;
            network[u][a.v].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;

            if (active && 0 < excess[a.u] && excess[a.u] <= delta) st.emplace_front(a.u);
        };

        auto relabel = [&](int v, T delta) {
            if (delta < inf) phi[v] -= delta + bound;
            else {
                phi[v] -= bound;
                count[v]--;
            }
        };

        auto scaled_cost = [&](int v, const Arc &a) {
            int diff = count[v] - count[a.u];
            if (diff > 0) return inf;
            else if (diff < 0) return -inf;
            else return a.cost + phi[v] - phi[a.u];
        };

        auto check = [&](int v) {
            if (abs(excess[v]) > 0) return false;

            T delta = inf;
            for (auto &&a : network[v]) {
                if (a.cap <= 0) continue;

                T c = scaled_cost(v, a);
                if (c < 0) return false;
                else delta = min(delta, c);
            }

            relabel(v, delta);
            return true;
        };

        auto discharge = [&](int v) {
            T delta = inf;

            for (auto a = network[v].begin(); a != network[v].end(); a++) {
                if (a->cap <= 0) continue;

                if (scaled_cost(v, *a) < 0) {
                    if (check(a->u)) {
                        a--;
                        continue;
                    }

                    push(v, *a, excess[v], true);
                    if (abs(excess[v]) <= 0) return;
                } else delta = min(delta, scaled_cost(v, *a));
            }

            relabel(v, delta);
            st.emplace_front(v);
        };

        while (bound > 1) {
            bound = max(bound >> shift, (T) 1);
            st.clear();

            for (int v = 0; v < n; v++)
                for (auto &&a : network[v])
                    if (scaled_cost(v, a) < 0 && a.cap > 0) push(v, a, a.cap, false);

            for (int v = 0; v < n; v++)
                if (excess[v] > 0) st.emplace_front(v);

            while (!st.empty()) {
                int v = st.front();
                st.pop_front();

                discharge(v);
            }
        }

        for (int v = 0; v < n; v++)
            for (auto &&a : network[v]) {
                a.cost >>= shift;
                cost -= a.cost * a.cap;
            }

        return {flow, cost /= 2};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;

    FlowNetwork<long long> fn(n);
    while (m--) {
        int u, v, c, w;
        cin >> u >> v >> c >> w;

        fn.add_arc(u, v, c, w);
    }

    auto [f, c] = fn.max_flow_min_cost(s, t);
    cout << f << " " << c;
}
