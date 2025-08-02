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

constexpr int MODULO = 1e6 + 3;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, Q;
    cin >> n >> m >> Q;

    vector<char> mirror(n + m + Q + 1, '#');
    vector<array<int, 4>> next(n + m + Q + 1, 0);
    unordered_map<pair<int, int>, int, Hash> indices;
    auto id = [&](int x, int y) {
        if (!indices.count({x, y})) return 0;
        return indices[{x, y}];
    };

    unordered_map<int, set<int>> rows, cols;
    auto x_lb = [&](int x, int y) {
        if (!rows.count(y)) return INT_MIN;

        auto &s = rows[y];
        auto it = s.lower_bound(x);
        if (it == s.begin()) return INT_MIN;
        return *prev(it);
    };

    auto x_ub = [&](int x, int y) {
        if (!rows.count(y)) return INT_MAX;

        auto &s = rows[y];
        auto it = s.upper_bound(x);
        if (it == s.end()) return INT_MAX;
        return *it;
    };

    auto add_row = [&](int x, int y, int i) {
        rows[y].emplace(x);
        int xl = x_lb(x, y), xr = x_ub(x, y),
            l = xl == INT_MIN ? 0 : id(xl, y), r = xr == INT_MAX ? 0 : id(xr, y);

        next[i][2] = l;
        next[i][0] = r;
        if (l) next[l][0] = i;
        if (r) next[r][2] = i;
    };

    auto remove_row = [&](int x, int y) {
        int xl = x_lb(x, y), xr = x_ub(x, y),
            l = xl == INT_MIN ? 0 : id(xl, y), r = xr == INT_MAX ? 0 : id(xr, y);

        if (l) next[l][0] = r;
        if (r) next[r][2] = l;
        rows[y].erase(x);
    };

    auto y_lb = [&](int x, int y) {
        if (!cols.count(x)) return INT_MIN;

        auto &s = cols[x];
        auto it = s.lower_bound(y);
        if (it == s.begin()) return INT_MIN;
        return *prev(it);
    };

    auto y_ub = [&](int x, int y) {
        if (!cols.count(x)) return INT_MAX;

        auto &s = cols[x];
        auto it = s.upper_bound(y);
        if (it == s.end()) return INT_MAX;
        return *it;
    };

    auto add_col = [&](int x, int y, int i) {
        cols[x].emplace(y);
        int yd = y_lb(x, y), yu = y_ub(x, y),
            d = yd == INT_MIN ? 0 : id(x, yd), u = yu == INT_MAX ? 0 : id(x, yu);

        next[i][3] = d;
        next[i][1] = u;
        if (d) next[d][1] = i;
        if (u) next[u][3] = i;
    };

    auto remove_col = [&](int x, int y) {
        int yd = y_lb(x, y), yu = y_ub(x, y),
            d = yd == INT_MIN ? 0 : id(x, yd), u = yu == INT_MAX ? 0 : id(x, yu);

        if (d) next[d][1] = u;
        if (u) next[u][3] = d;
        cols[x].erase(y);
    };

    auto add = [&](int x, int y, int i) {
        indices[{x, y}] = i;
        add_row(x, y, i);
        add_col(x, y, i);
    };

    auto remove = [&](int x, int y) {
        remove_row(x, y);
        remove_col(x, y);
        indices.erase({x, y});
    };

    vector<pair<int, int>> monkey(n + 1);
    for (int i = 1; i <= n; i++) {
        int x, y;
        cin >> x >> y;

        mirror[i] = '#';
        add(x, y, i);
        monkey[i] = {x, y};
    }

    int count = n;
    while (m--) {
        int x, y;
        char c;
        cin >> x >> y >> c;

        int i = ++count;
        mirror[i] = c;
        add(x, y, i);
    }

    int s = 0;
    vector<int> back{3, 2, 1, 0}, front{1, 0, 3, 2};
    unordered_map<char, int> dtoi{{'E', 0}, {'N', 1}, {'W', 2}, {'S', 3}};
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i, x, y;
            cin >> i >> x >> y;
            x = (x + s) % MODULO;
            y = (y + s) % MODULO;

            auto [xt, yt] = monkey[i];
            remove(xt, yt);
            mirror[i] = '#';
            add(x, y, i);
            monkey[i] = {x, y};
        } else if (q == 2) {
            int x, y;
            char c;
            cin >> x >> y >> c;
            x = (x + s) % MODULO;
            y = (y + s) % MODULO;

            if (c == '.') remove(x, y);
            else if (indices.count({x, y})) mirror[indices[{x, y}]] = c;
            else {
                int i = ++count;
                mirror[i] = c;
                add(x, y, i);
            }
        } else {
            int i;
            char d;
            cin >> i >> d;

            auto zap = [&](int v, int dir1) {
                int u = v, dir2 = dir1;
                for (;;) {
                    int k = next[u][dir2];
                    if (!k) return -1;
                    if (mirror[k] == '#') return k;

                    u = k;
                    dir2 = mirror[k] == '\\' ? back[dir2] : front[dir2];
                }
            };

            unordered_set<pair<int, int>, Hash> seen;
            int k = zap(indices[monkey[i]], dtoi[d]);
            cout << k << "\n";
            s = (s + k + MODULO) % MODULO;
        }
    }
}
