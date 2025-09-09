#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto make_turn = [&](int d, char c) {
        if (c == '>') return (d + 1) & 3;
        if (c == 'U') return (d + 2) & 3;
        return (d + 3) & 3;
    };

    unordered_map<int, vector<pair<int, int>>> x_walks, y_walks;
    auto add_h = [&](int y, int x1, int x2) {
        x_walks[y].emplace_back(minmax(x1, x2));
    };
    auto add_v = [&](int x, int y1, int y2) {
        y_walks[x].emplace_back(minmax(y1, y2));
    };

    int dir = 0, ex = 0, ey = 0;
    while (n--) {
        string s;
        cin >> s;

        if (!isdigit(s[0])) dir = make_turn(dir, s[0]);
        else {
            int k = stoi(s), x = ex, y = ey;
            if (!dir) x += k;
            else if (dir == 1) y -= k;
            else if (dir == 2) x -= k;
            else y += k;

            if (dir & 1) add_v(ex, ey, y);
            else add_h(ey, ex, x);

            ex = x;
            ey = y;
        }
    }

    if (!ex && !ey) {
        cout << "exit";
        exit(0);
    }

    auto merge = [&](auto &v) {
        sort(v.begin(), v.end());
        vector<pair<int, int>> temp;
        for (auto [l, r] : v) {
            if (temp.empty() || l > temp.back().second + 1) temp.emplace_back(l, r);
            else temp.back().second = max(temp.back().second, r);
        }
        v = temp;
    };

    for (auto &[y, walks] : x_walks) merge(walks);
    for (auto &[x, walks] : y_walks) merge(walks);

    auto collect = [&](const auto &z_walks, auto &v1, auto &v2) {
        for (auto [z, walks] : z_walks) {
            v1.emplace_back(z);
            for (auto [l, r] : walks) {
                v2.emplace_back(l);
                v2.emplace_back(r);
            }
        }
    };

    vector<int> xs{0, ex}, ys{0, ey};
    collect(x_walks, ys, xs);
    collect(y_walks, xs, ys);

    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
    };
    dedupe(xs);
    dedupe(ys);

    vector<pair<int, int>> coords;
    unordered_map<pair<int, int>, int, Hash> id;
    auto add = [&](int x, int y) -> int {
        if (id.count({x, y})) return id[{x, y}];
        coords.emplace_back(x, y);
        return id[{x, y}] = coords.size() - 1;
    };

    unordered_map<int, vector<int>> adj_list_x, adj_list_y;
    add(0, 0);
    add(ex, ey);
    adj_list_x[0].emplace_back(0);
    adj_list_x[ex].emplace_back(ey);
    adj_list_y[0].emplace_back(0);
    adj_list_y[ey].emplace_back(ex);
    for (auto [y, walks] : x_walks)
        for (auto [xl, xr] : walks) {
            add(xl, y);
            add(xr, y);
            adj_list_x[xl].emplace_back(y);
            adj_list_x[xr].emplace_back(y);
            adj_list_y[y].emplace_back(xl);
            adj_list_y[y].emplace_back(xr);
        }

    for (auto [x, walks] : y_walks)
        for (auto [yl, yr] : walks) {
            add(x, yl);
            add(x, yr);
            adj_list_x[x].emplace_back(yl);
            adj_list_x[x].emplace_back(yr);
            adj_list_y[yl].emplace_back(x);
            adj_list_y[yr].emplace_back(x);
        }

    auto within = [&](auto &walks, int z) {
        int l = -1, r = walks.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (walks[m].first > z) r = m;
            else l = m;
        }
        if (!~l) return false;

        auto [zl, zr] = walks[l];
        return zl <= z && z <= zr;
    };

    for (int y : ys)
        for (int x : xs)
            if (y_walks.count(x) && within(y_walks[x], y)) {
                add(x, y);
                adj_list_x[x].emplace_back(y);
                adj_list_y[y].emplace_back(x);
            }

    for (int x : xs)
        for (int y : ys)
            if (x_walks.count(y) && within(x_walks[y], x)) {
                add(x, y);
                adj_list_x[x].emplace_back(y);
                adj_list_y[y].emplace_back(x);
            }

    for (auto &[x, y_neighbors] : adj_list_x) dedupe(y_neighbors);
    for (auto &[y, x_neighbors] : adj_list_y) dedupe(x_neighbors);

    unordered_map<int, vector<pair<int, int>>> x_safe, y_safe;
    for (int y : ys) {
        vector<pair<int, int>> walks;
        if (x_walks.count(y)) walks = x_walks[y];

        for (int x : xs)
            if (y_walks.count(x) && within(y_walks[x], y)) walks.emplace_back(x, x);

        if (!walks.empty()) {
            merge(walks);
            x_safe[y] = walks;
        }
    }

    for (int x : xs) {
        vector<pair<int, int>> walks;
        if (y_walks.count(x)) walks = y_walks[x];

        for (int y : ys)
            if (x_walks.count(y) && within(x_walks[y], x)) walks.emplace_back(y, y);

        if (!walks.empty()) {
            merge(walks);
            y_safe[x] = walks;
        }
    }

    vector<vector<pair<int, int>>> adj_list(coords.size());
    auto add_edge = [&](int u, int v, int w) {
        if (u == v || w <= 0) return;
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    };
    for (auto [y, x_neighbors] : adj_list_y)
        if (x_safe.count(y))
            for (auto [xl, xr] : x_safe[y]) {
                auto it_l = lower_bound(x_neighbors.begin(), x_neighbors.end(), xl),
                     it_r = upper_bound(x_neighbors.begin(), x_neighbors.end(), xr);
                if (it_l == it_r) continue;

                int x_prev = *it_l, i = id[{x_prev, y}];
                for (it_l++; it_l != it_r; it_l++) {
                    int x_curr = *it_l, j = id[{x_curr, y}];
                    add_edge(i, j, x_curr - x_prev);
                    x_prev = x_curr;
                    i = j;
                }
            }

    for (auto [x, y_neighbors] : adj_list_x)
        if (y_safe.count(x))
            for (auto [yd, yu] : y_safe[x]) {
                auto it_d = lower_bound(y_neighbors.begin(), y_neighbors.end(), yd),
                     it_u = upper_bound(y_neighbors.begin(), y_neighbors.end(), yu);
                if (it_d == it_u) continue;

                int y_prev = *it_d, i = id[{x, y_prev}];
                for (it_d++; it_d != it_u; it_d++) {
                    int y_curr = *it_d, j = id[{x, y_curr}];
                    add_edge(i, j, y_curr - y_prev);
                    y_prev = y_curr;
                    i = j;
                }
            }

    int src = id[{0, 0}], dest = id[{ex, ey}];
    vector<int> dist(coords.size(), 1e9), prev(coords.size(), -1);
    dist[dest] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, dest);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        if (v == src) break;

        for (auto [u, w] : adj_list[v])
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
                prev[u] = v;
            }
    }

    vector<int> path;
    for (int v = src; ~v; v = prev[v]) path.emplace_back(v);
    reverse(path.begin(), path.end());

    auto get_turn = [&](int d1, int d2) -> string {
        int d = (d2 - d1) & 3;
        if (!d) return "";
        if (d == 1) return ">";
        if (d == 2) return "U";
        return "<";
    };

    int walk = 0;
    for (int i = 0; i + 1 < path.size(); i++) {
        auto a = coords[path[i]], b = coords[path[i + 1]];

        int next_dir;
        if (a.first < b.first) next_dir = 0;
        else if (a.second > b.second) next_dir = 1;
        else if (a.first > b.first) next_dir = 2;
        else next_dir = 3;

        int w = abs(a.first - b.first) + abs(a.second - b.second);
        if (dir == next_dir) walk += w;
        else {
            if (walk) cout << to_string(walk) << " ";
            walk = w;
            if (dir != next_dir) {
                cout << get_turn(dir, next_dir) << " ";
                dir = next_dir;
            }
        }
    }
    if (walk) cout << to_string(walk) << " ";
    cout << "exit";
}
