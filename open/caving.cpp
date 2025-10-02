#include <bits/stdc++.h>
using namespace std;

struct StaticTopTree {
    static inline vector<int> vals;

    enum Operation {
        NA = 0,
        LeafVertex = 1,
        InnerVertex = 2,
        Edge = 3,
        Rake = 4,
        Compress = 5
    };

    struct StaticTopTreeNode {
        array<int, 3> family;
        Operation op;

        StaticTopTreeNode() : family{-1, -1, -1}, op(NA) {}
    };

    struct Path {
        struct Monoid {
            long long sum, pref, suff, subpath;

            Monoid(long long v = 0) {
                sum = v;
                pref = suff = subpath = max(0LL, v);
            }

            friend auto operator+(const Monoid &ml, const Monoid &mr) {
                Monoid monoid;
                monoid.sum = ml.sum + mr.sum;
                monoid.pref = max(ml.pref, ml.sum + mr.pref);
                monoid.suff = max(ml.suff + mr.sum, mr.suff);
                monoid.subpath = max(max(ml.subpath, mr.subpath), ml.suff + mr.pref);
                return monoid;
            }
        };

        Monoid m;
        long long aggregate;

        Path(const Monoid &m = {0}, const long long aggregate = 0) : m(m), aggregate(aggregate) {}
    };

    struct Point {
        long long light_sum, base;

        Point(const long long light_sum = 0, const long long base = 0) : light_sum(light_sum), base(base) {}
    };

    int root, nodes;
    vector<StaticTopTreeNode> STT;
    vector<Path> dp_path;
    vector<Point> dp_point;
    vector<vector<int>> adj_list;

    StaticTopTree(int n, vector<vector<int>> &adj_list, const vector<int> &values) : root(-1), nodes(n), STT(4 * n),
                                                                                     dp_path(4 * n), dp_point(4 * n),
                                                                                     adj_list(adj_list) {
        vals = values;
        auto hld = [&](auto &&self, int v = 0) -> int {
            int subtree_size = 1, largest = 0;
            for (int &u : adj_list[v]) {
                int size = self(self, u);
                subtree_size += size;
                if (largest < size) {
                    largest = size;
                    swap(u, adj_list[v][0]);
                }
            }
            return subtree_size;
        };
        hld(hld);
        dfs(root = build_compress(0).first);
    }

    int node(int l, int r, Operation o, int i = -1) {
        if (!~i) i = nodes++;
        STT[i].family = {l, r, -1};
        STT[i].op = o;
        if (~l) STT[l].family[2] = i;
        if (~r) STT[r].family[2] = i;
        return i;
    }

    pair<int, int> build(vector<pair<int, int>> &path, Operation o) {
        if (path.size() == 1) return path[0];

        int total = 0;
        for (auto [i, s] : path) total += s;
        vector<pair<int, int>> pl, pr;
        for (auto [i, s] : path) {
            if (s < total) pl.emplace_back(i, s);
            else pr.emplace_back(i, s);
            total -= 2 * s;
        }

        if (pl.empty()) return build(pr, o);
        if (pr.empty()) return build(pl, o);
        auto [l, sl] = build(pl, o);
        auto [r, sr] = build(pr, o);
        return {node(l, r, o), sl + sr};
    }

    pair<int, int> build_vertex(int v) {
        auto [l, s] = build_rake(v);
        return {node(l, -1, !~l ? LeafVertex : InnerVertex, v), s + 1};
    }

    pair<int, int> build_edge(int v) {
        auto [l, s] = build_compress(v);
        return {node(l, -1, Edge), s};
    }

    pair<int, int> build_rake(int v) {
        vector<pair<int, int>> path;
        for (int u : adj_list[v])
            if (u != adj_list[v][0]) path.emplace_back(build_edge(u));
        if (path.empty()) return {-1, 0};
        return build(path, Rake);
    }

    pair<int, int> build_compress(int v) {
        vector<pair<int, int>> path;
        path.emplace_back(build_vertex(v));
        while (!adj_list[v].empty()) path.emplace_back(build_vertex(v = adj_list[v][0]));
        return build(path, Compress);
    }

    Path leaf(int i) {
        return {vals[i]};
    }

    Path inner(int i, const Point &p) {
        return {vals[i] + p.light_sum, p.base};
    }

    Point edge(const Path &p) {
        return {max(0LL, p.m.pref), max(p.m.subpath, p.aggregate)};
    }

    Point rake(const Point &pl, const Point &pr) {
        return {pl.light_sum + pr.light_sum, max(pl.base, pr.base)};
    }

    Path compress(const Path &pl, const Path &pr) {
        return {pl.m + pr.m, max(pl.aggregate, pr.aggregate)};
    }

    void push(int i) {
        auto [l, r, p] = STT[i].family;
        switch (STT[i].op) {
            case LeafVertex: {
                dp_path[i] = leaf(i);
                return;
            }
            case InnerVertex: {
                dp_path[i] = inner(i, dp_point[l]);
                return;
            }
            case Edge: {
                dp_point[i] = edge(dp_path[l]);
                return;
            }
            case Rake: {
                dp_point[i] = rake(dp_point[l], dp_point[r]);
                return;
            }
            case Compress: {
                dp_path[i] = compress(dp_path[l], dp_path[r]);
                return;
            }
            default: return;
        }
    }

    void dfs(int i) {
        auto [l, r, p] = STT[i].family;
        if (~l) dfs(l);
        if (~r) dfs(r);
        push(i);
    }

    long long dp(int i) {
        auto val = max(dp_path[root].m.subpath, dp_path[root].aggregate);
        vals[i] = 0;
        while (~i) {
            push(i);
            i = STT[i].family[2];
        }
        return val;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int max_v = INT_MIN;
    vector<int> values(n);
    for (int &vi : values) {
        cin >> vi;

        max_v = max(max_v, vi);
    }

    if (max_v < 0) {
        cout << "0 1";
        exit(0);
    }

    if (!max_v) {
        cout << "0 0";
        exit(0);
    }

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<vector<int>> adj_list_tree(n);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (int u : adj_list[v])
            if (u != prev) {
                adj_list_tree[v].emplace_back(u);
                self(self, u, v);
            }
    };
    dfs(dfs);

    StaticTopTree stt(n, adj_list_tree, values);
    vector<long long> v(n);
    for (int d = 0; d < n; d++) v[d] = stt.dp(d);

    pair<long long, int> p{0, -n};
    auto &[val, day] = p;
    for (int d = 0; d < n; d++) p = max(p, {v[d], -d});
    cout << val << " " << -day;
}
