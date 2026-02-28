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

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;

        SplayNode() : family{0, 0, 0} {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {}

    auto & operator[](int i) {
        return ST[i];
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) { return ST[parent].family[1] == i; };

        auto rotate = [&](int i) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) ST[k].family[child(j, k)] = i;

            int c = child(i, j), s = ST[j].family[c] = ST[i].family[c ^ 1];
            if (s) ST[s].family[2] = j;

            ST[i].family[c ^ 1] = j;
            ST[i].family[2] = k;
            ST[j].family[2] = i;
        };

        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
    }

    int subtree_min(int i) {
        while (ST[i].family[0]) i = ST[i].family[0];
        return i;
    }
};

struct RootedLinkCutTree : SplayTree {
    RootedLinkCutTree(int n) : SplayTree(n) {}

    void access(int i) {
        for (int u = 0, v = i; v; u = v, v = ST[v].family[2]) {
            splay(v);
            ST[v].family[1] = u;
        }
        splay(i);
    }

    int find(int i) {
        access(i);
        i = subtree_min(i);
        splay(i);
        return i;
    }

    void link(int i, int j) {
        access(i);
        ST[i].family[2] = j;
    }

    void cut(int i) {
        access(i);
        auto &[l, r, p] = ST[i].family;
        l = ST[l].family[2] = 0;
    }
};

constexpr int MOD = 1e6 + 3;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    int k = n + m + q;
    vector<char> mirror(k + 1, '.');
    vector<array<int, 4>> next(k + 1, {0, 0, 0, 0});
    unordered_map<pair<int, int>, int, Hash> indices;
    auto id = [&](int x, int y) {
        auto it = indices.find({x, y});
        if (it == indices.end()) return 0;
        return it->second;
    };

    unordered_map<int, set<int>> rows, cols;
    auto x_lb = [&](int x, int y) {
        auto it1 = rows.find(y);
        if (it1 == rows.end()) return INT_MIN;

        auto &s = it1->second;
        auto it2 = s.lower_bound(x);
        if (it2 == s.begin()) return INT_MIN;
        return *prev(it2);
    };

    auto x_ub = [&](int x, int y) {
        auto it1 = rows.find(y);
        if (it1 == rows.end()) return INT_MAX;

        auto &s = it1->second;
        auto it2 = s.upper_bound(x);
        if (it2 == s.end()) return INT_MAX;
        return *it2;
    };

    auto y_lb = [&](int x, int y) {
        auto it1 = cols.find(x);
        if (it1 == cols.end()) return INT_MIN;

        auto &s = it1->second;
        auto it2 = s.lower_bound(y);
        if (it2 == s.begin()) return INT_MIN;
        return *prev(it2);
    };

    auto y_ub = [&](int x, int y) {
        auto it1 = cols.find(x);
        if (it1 == cols.end()) return INT_MAX;

        auto &s = it1->second;
        auto it2 = s.upper_bound(y);
        if (it2 == s.end()) return INT_MAX;
        return *it2;
    };

    vector<int> back{3, 2, 1, 0}, front{1, 0, 3, 2};
    auto reflect = [&](int i, int dir) {
        return mirror[i] == '\\' ? back[dir] : front[dir];
    };

    auto node = [&](int i, int dir) {
        return 4 * i + dir + 1;
    };
    int nodes = 4 * (k + 1) + n + 1;
    RootedLinkCutTree lct(nodes);
    vector<int> parent(nodes + 1, 0), cycle(nodes + 1, 0);
    auto update = [&](int i, int dir) {
        int dir_reflected = reflect(i, dir), j = next[i][dir_reflected],
            u = node(i, dir), v = !j ? 4 * k + 1 : (j <= n ? 4 * k + 1 + j : node(j, dir_reflected));
        if (cycle[u]) cycle[u] = 0;
        else if (parent[u]) {
            lct.cut(u);
            int t = lct.find(parent[u]);
            parent[u] = 0;
            if (cycle[t] && lct.find(cycle[t]) != t) {
                lct.link(t, parent[t] = cycle[t]);
                cycle[t] = 0;
            }
        }
        if (!v) return;
        if (lct.find(v) != u) {
            lct.link(u, v);
            parent[u] = v;
        } else cycle[u] = v;
    };

    auto connect = [&](int i, int j, int dir) {
        next[i][dir] = j;
        if (i > n && mirror[i] != '.') update(i, reflect(i, dir));
    };

    auto add_row = [&](int x, int y, int i) {
        rows[y].emplace(x);
        int xl = x_lb(x, y), xr = x_ub(x, y),
            l = xl == INT_MIN ? 0 : id(xl, y), r = xr == INT_MAX ? 0 : id(xr, y);

        next[i][2] = l;
        next[i][0] = r;
        if (l) connect(l, i, 0);
        if (r) connect(r, i, 2);
    };

    auto remove_row = [&](int x, int y) {
        int xl = x_lb(x, y), xr = x_ub(x, y),
            l = xl == INT_MIN ? 0 : id(xl, y), r = xr == INT_MAX ? 0 : id(xr, y);

        if (l) connect(l, r, 0);
        if (r) connect(r, l, 2);
        rows[y].erase(x);
    };

    auto add_col = [&](int x, int y, int i) {
        cols[x].emplace(y);
        int yd = y_lb(x, y), yu = y_ub(x, y),
            d = yd == INT_MIN ? 0 : id(x, yd), u = yu == INT_MAX ? 0 : id(x, yu);

        next[i][3] = d;
        next[i][1] = u;
        if (d) connect(d, i, 1);
        if (u) connect(u, i, 3);
    };

    auto remove_col = [&](int x, int y) {
        int yd = y_lb(x, y), yu = y_ub(x, y),
            d = yd == INT_MIN ? 0 : id(x, yd), u = yu == INT_MAX ? 0 : id(x, yu);

        if (d) connect(d, u, 1);
        if (u) connect(u, d, 3);
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

        monkey[i] = {x, y};
        add(x, y, i);
    }

    int count = n;
    while (m--) {
        int x, y;
        char c;
        cin >> x >> y >> c;

        int i = ++count;
        mirror[i] = c;
        add(x, y, i);
        for (int dir = 0; dir < 4; dir++) update(i, dir);
    }

    int s = 0;
    unordered_map<char, int> dtoi{{'E', 0}, {'N', 1}, {'W', 2}, {'S', 3}};
    vector<int> zap(nodes + 1, 0);
    zap[4 * k + 1] = -1;
    iota(zap.begin() + 4 * k + 2, zap.end(), 1);
    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int i, x, y;
            cin >> i >> x >> y;
            x = (x + s) % MOD;
            y = (y + s) % MOD;

            auto [xt, yt] = monkey[i];
            remove(xt, yt);
            mirror[i] = '.';
            add(x, y, i);
            monkey[i] = {x, y};
        } else if (type == 2) {
            int x, y;
            char c;
            cin >> x >> y >> c;
            x = (x + s) % MOD;
            y = (y + s) % MOD;

            if (c == '.') remove(x, y);
            else {
                int i = id(x, y);
                if (!i) {
                    i = ++count;
                    add(x, y, i);
                }
                mirror[i] = c;
                if (i <= n) continue;
                for (int dir = 0; dir < 4; dir++) update(i, dir);
            }
        } else {
            int i;
            char d;
            cin >> i >> d;

            auto [x, y] = monkey[i];
            int hit, dir = dtoi[d];
            if (!dir) hit = id(x_ub(x, y), y);
            else if (dir == 1) hit = id(x, y_ub(x, y));
            else if (dir == 2) hit = id(x_lb(x, y), y);
            else hit = id(x, y_lb(x, y));

            if (!hit) hit = -1;
            else if (hit > n) hit = zap[lct.find(node(hit, dir))];

            cout << hit << "\n";
            s = (s + hit + MOD) % MOD;
        }
    }
}
