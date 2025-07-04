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
                    int hv = height[v];
                    height[v] = INT_MAX;

                    for (auto a = network[v].begin(); a != network[v].end(); a++)
                        if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                    count[height[v]]++;
                    if (!--count[hv] && hv < n)
                        for (int u = 0; u < n; u++) relabel(u, hv);
                } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                else curr[v]++;

            if (h) h--;
            while (h < 2 * n && active_stacks[h].empty()) h++;
        };

        if (!active_stacks[0].empty())
            for (int h = 0; h < 2 * n;) {
                int v = active_stacks[h].top();
                active_stacks[h].pop();

                discharge(v, h);
            }

        return -excess[s];
    }

    vector<pair<int, int>> min_cut(int s, const vector<array<int, 4>> &edges) {
        vector<bool> visited(n, false);
        visited[s] = true;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, _, cap, __] : network[v])
                if (cap > 0 && !visited[u]) {
                    visited[u] = true;
                    q.emplace(u);
                }
        }

        vector<pair<int, int>> cut_edges;
        for (auto [u, v, i, j] : edges)
            if (visited[u] && !visited[v]) cut_edges.emplace_back(i, j);
        return cut_edges;
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

    int n;
    cin >> n;

    int s = 2 * n * n, t = 2 * n * n + 1;
    FlowNetwork<int> fn(2 * n * n + 2);
    vector<vector<int>> in(n, vector<int>(n)), out(n, vector<int>(n));
    int count = 0;
    for (auto &row : in)
        for (auto &cell : row) cell = count++;

    for (auto &row : out)
        for (auto &cell : row) cell = count++;

    vector<array<int, 3>> oil;
    vector<array<int, 4>> split;
    vector<array<int, 5>> interior, border;
    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            char ch;
            cin >> ch;

            if (ch == 'O') {
                fn.add_arc(s, out[i][j], 1e9, 0);
                oil.push_back({(int) fn.network[s].size() - 1, i, j});
            }

            for (int k = 0; k < 4; k++) {
                int r = i + dx[k], c = j + dy[k];
                if (0 <= r && r < n && 0 <= c && c < n) {
                    fn.add_arc(in[i][j], out[r][c], 1, 0);
                    interior.push_back({(int) fn.network[in[i][j]].size() - 1, i, j, r, c});
                } else if (!i || i == n - 1 || !j || j == n - 1) {
                    fn.add_arc(in[i][j], t, 1e9, 0);
                    border.push_back({(int) fn.network[in[i][j]].size() - 1, i, j, r, c});
                }
            }

            fn.add_arc(out[i][j], in[i][j], 1, 0);
            split.push_back({out[i][j], in[i][j], i, j});
        }
    fn.max_flow(s, t);

    vector<array<int, 4>> rectangles;
    for (auto [i, j] : fn.min_cut(s, split)) rectangles.push_back({i, i, j, j});
    bool change;
    do {
        change = false;
        for (int i = 0; i < rectangles.size(); i++)
            for (int j = i + 1; j < rectangles.size(); j++) {
                auto [xl1, xr1, yd1, yu1] = rectangles[i];
                auto [xl2, xr2, yd2, yu2] = rectangles[j];
                if (max(xl1, xl2) <= min(xr1, xr2) + 1 && max(yd1, yd2) <= min(yu1, yu2) + 1) {
                    rectangles.erase(rectangles.begin() + j);
                    rectangles.erase(rectangles.begin() + i);
                    rectangles.push_back({min(xl1, xl2), max(xr1, xr2), min(yd1, yd2), max(yu1, yu2)});
                    change = true;
                    goto next;
                }
            }
        next:;
    } while (change);

    DisjointSets dsu(n * n);
    vector<array<int, 4>> candidates;
    for (auto [e, i, j, r, c] : interior)
        if (fn.network[in[i][j]][e].cap < fn.network[in[i][j]][e].initial_cap) {
            dsu.unite(i * n + j, r * n + c);
            candidates.push_back({i, j, r, c});
        }

    int size = 4 * n + 1;
    vector<string> config(size, string(size, '.'));
    vector<bool> visited(n * n, false);
    for (auto [e, i, j] : oil)
        if (fn.network[s][e].cap < fn.network[s][e].initial_cap) {
            visited[dsu.find(i * n + j)] = true;
            config[i * 4 + 2][j * 4 + 2] = 'O';
        }

    auto add_pipe = [&](int i, int j, int r, int c) {
        int x1 = i * 4 + 2, x2 = r * 4 + 2, y1 = j * 4 + 2, y2 = c * 4 + 2;
        auto [xl, xr] = minmax(x1, x2);
        auto [yd, yu] = minmax(y1, y2);
        for (int x = xl; x <= xr; x++)
            if (0 <= x && x < size)
                for (int y = yd; y <= yu; y++)
                    if (0 <= y && y < size && config[x][y] != 'O') config[x][y] = '+';
    };

    for (auto [i, j, r, c] : candidates)
        if (visited[dsu.find(i * n + j)]) add_pipe(i, j, r, c);

    for (auto [e, i, j, r, c] : border)
        if (fn.network[in[i][j]][e].cap < fn.network[in[i][j]][e].initial_cap) add_pipe(i, j, r, c);

    for (auto [xl, xr, yd, yu] : rectangles)
        for (int x = xl * 4; x <= xr * 4 + 4; x++)
            if (0 <= x && x < size)
                for (int y = yd * 4; y <= yu * 4 + 4; y++)
                    if (0 <= y && y < size)
                        if (x == xl * 4 || x == xr * 4 + 4 || y == yd * 4 || y == yu * 4 + 4) config[x][y] = '#';

    for (auto &row : config) cout << row << "\n";
}
