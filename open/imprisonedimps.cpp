#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, rev;
        T cap, initial_cap;
        Arc(int u, int rev, T cap) : u(u), rev(rev), cap(cap), initial_cap(cap) {}
    };

    int n;
    vector<vector<Arc>> network;
    vector<int> dist;
    vector<typename vector<Arc>::iterator> it;

    FlowNetwork(int n) : n(n), network(n), dist(n), it(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    bool bfs(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        dist[s] = 0;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, _, cap, __] : network[v])
                if (cap > 0 && !~dist[u]) {
                    dist[u] = dist[v] + 1;
                    q.emplace(u);
                }
        }
        return ~dist[t];
    }

    T dfs(int v, int t, T flow) {
        if (v == t) return flow;

        for (; it[v] != network[v].end(); it[v]++) {
            auto &[u, rev, cap, _] = *it[v];
            if (cap > 0 && dist[u] == dist[v] + 1) {
                T f = dfs(u, t, min(flow, cap));
                if (f > 0) {
                    cap -= f;
                    network[u][rev].cap += f;
                    return f;
                }
            }
        }
        return (T) 0;
    }

    T max_flow(int s, int t) {
        T flow = 0, f;
        while (bfs(s, t)) {
            for (int v = 0; v < n; v++) it[v] = network[v].begin();
            while ((f = dfs(s, t, numeric_limits<T>::max())) > 0) flow += f;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int I, p;
    cin >> I >> p;

    vector<array<int, 3>> imps(I);
    for (auto &[x, y, s] : imps) cin >> x >> y >> s;

    vector<int> cx(p), cy(p);
    vector<vector<int>> outflows(p);
    vector<vector<bool>> wall(1e3 + 1, vector<bool>(1e3 + 1, false));
    for (int i = 0; i < p; i++) {
        int o;
        cin >> cx[i] >> cy[i] >> o;

        wall[cx[i]][cy[i]] = true;
        outflows[i].resize(o);
        for (int &v : outflows[i]) cin >> v;
    }

    auto mark = [&](int x, int y) {
        if (0 <= x && x <= 1e3 && 0 <= y && y <= 1e3) wall[x][y] = true;
    };

    vector<array<int, 5>> hori, verti;
    for (int u = 0; u < p; u++)
        for (int v : outflows[u])
            if (cx[u] != cx[v]) {
                auto [l, r] = minmax(cx[u], cx[v]);
                for (int x = l + 1; x < r; x++) mark(x, cy[u]);
                hori.push_back({u, v, cy[u], l, r});
            } else {
                auto [l, r] = minmax(cy[u], cy[v]);
                for (int y = l + 1; y < r; y++) mark(cx[u], y);
                verti.push_back({u, v, cx[u], l, r});
            }

    queue<pair<int, int>> q;
    vector<vector<int>> spell(1e3 + 1, vector<int>(1e3 + 1, 0));
    vector<vector<bool>> visited(1e3 + 1, vector<bool>(1e3 + 1, false));
    for (auto [x, y, s] : imps)
        if (0 <= x && x <= 1e3 && 0 <= y && y <= 1e3 && !wall[x][y]) {
            spell[x][y] = max(spell[x][y], s);
            if (!visited[x][y]) {
                visited[x][y] = true;
                q.emplace(x, y);
            }
        }

    auto visit = [&](int x, int y, int s) {
        if (!(0 <= x && x <= 1e3 && 0 <= y && y <= 1e3) || wall[x][y] || visited[x][y]) return;
        spell[x][y] = s;
        visited[x][y] = true;
        q.emplace(x, y);
    };
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        visit(x + 1, y, spell[x][y]);
        visit(x - 1, y, spell[x][y]);
        visit(x, y + 1, spell[x][y]);
        visit(x, y - 1, spell[x][y]);
    }

    vector<int> s_h(hori.size()), s_v(verti.size());
    for (int i = 0; i < hori.size(); i++) {
        auto [u, v, y, l, r] = hori[i];
        for (int x = l; x < r; x++) {
            int s = 0;
            if (y - 1 >= 0) s = max(s, spell[x][y - 1]);
            if (y + 1 <= 1e3) s = max(s, spell[x][y + 1]);
            s_h[i] = max(s_h[i], s);
        }
    }
    for (int i = 0; i < verti.size(); i++) {
        auto [u, v, x, l, r] = verti[i];
        for (int y = l; y < r; y++) {
            int s = 0;
            if (x - 1 >= 0) s = max(s, spell[x - 1][y]);
            if (x + 1 <= 1e3) s = max(s, spell[x + 1][y]);
            s_v[i] = max(s_v[i], s);
        }
    }

    vector<int> s(p, 0);
    auto update = [&](int x, int y, int v) {
        if (!(0 <= x && x <= 1e3 && 0 <= y && y <= 1e3) || wall[x][y]) return;
        s[v] = max(s[v], spell[x][y]);
    };
    for (int v = 0; v < p; v++) {
        int x = cx[v], y = cy[v];
        update(x + 1, y, v);
        update(x - 1, y, v);
        update(x, y + 1, v);
        update(x, y - 1, v);
    }

    vector<int> delta(2 * p, 0);
    FlowNetwork<long long> fn(2 * p + 2);
    auto add = [&](int u, int v, int s) {
        delta[u] -= s;
        delta[v] += s;
        fn.add_arc(u, v, 1e6);
    };

    for (int v = 0; v < p; v++) add(v, v + p, s[v]);
    for (int i = 0; i < hori.size(); i++) add(hori[i][0] + p, hori[i][1], s_h[i]);
    for (int i = 0; i < verti.size(); i++) add(verti[i][0] + p, verti[i][1], s_v[i]);
    fn.add_arc(2 * p - 1, 0, 1e6);

    for (int i = 0; i < 2 * p; i++)
        if (delta[i] > 0) fn.add_arc(2 * p, i, delta[i]);
        else if (delta[i] < 0) fn.add_arc(i, 2 * p + 1, -delta[i]);

    fn.max_flow(2 * p, 2 * p + 1);

    for (auto [u, _, cap, initial_cap] : fn.network[2 * p - 1])
        if (!u) {
            cout << initial_cap - cap;
            exit(0);
        }
}
