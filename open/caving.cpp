#include <bits/stdc++.h>
using namespace std;

struct StaticTopTree {
    enum Operation {
        NA = 0,
        LeafVertex = 1,
        InnerVertex = 2,
        Edge = 3,
        Rake = 4,
        Compress = 5
    };

    struct Path {
        struct Monoid {
            long long sum, pref, suff, subpath;

            Monoid(long long v = 0) {
                sum = pref = suff = subpath = v;
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

        Path(const Monoid &m = {0}, long long aggregate = LLONG_MIN) : m(m), aggregate(aggregate) {}
    };

    struct Point {
        long long light_sum, base;

        Point(long long sum = 0LL, long long base = LLONG_MIN) : light_sum(sum), base(base) {}
    };

    struct StaticTopTreeNode {
        array<int, 3> family;
        int size;
        Operation op;
        Path path;
        Point point;

        StaticTopTreeNode() : family{-1, -1, -1}, size(0), op(NA) {}
    };

    int root, nodes;
    vector<StaticTopTreeNode> STT;
    vector<int> vals;

    StaticTopTree(int n, vector<vector<int>> &adj_list, const vector<int> &values) : root(-1), nodes(n), STT(4 * n), vals(values) {
        vector<int> heavy(n, -1);
        auto hld = [&](auto &&self, int v = 0) -> int {
            int subtree_size = 1, largest = 0;
            for (int &u : adj_list[v]) {
                int size = self(self, u);
                subtree_size += size;
                if (largest < size) {
                    largest = size;
                    heavy[v] = u;
                    swap(u, adj_list[v][0]);
                }
            }
            return subtree_size;
        };
        hld(hld);

        auto build = [&](auto &&self, vector<int> &clusters, int l, int r, Operation o) -> int {
            if (l + 1 == r) return clusters[l];

            int sum = 0;
            for (int i = l; i < r; i++) sum += STT[clusters[i]].size;

            int m = r - 1;
            for (int i = l, s = 0; i < m; i++) {
                s += STT[clusters[i]].size;
                if (2 * s >= sum) {
                    m = i + 1;
                    break;
                }
            }

            return node(self(self, clusters, l, m, o), self(self, clusters, m, r, o), o);
        };

        auto dfs = [&](auto &&self, int v = 0) -> int {
            vector<int> path_clusters;
            for (int u = v; ~u; u = heavy[u]) {
                vector<int> point_clusters;
                for (int i = 1; i < adj_list[u].size(); i++) point_clusters.emplace_back(node(self(self, adj_list[u][i]), -1, Edge));

                int l = -1;
                if (!point_clusters.empty()) l = build(build, point_clusters, 0, point_clusters.size(), Rake);
                path_clusters.emplace_back(node(l, -1, !~l ? LeafVertex : InnerVertex, u));
            }
            return build(build, path_clusters, 0, path_clusters.size(), Compress);
        };
        root = dfs(dfs);
    }

    int node(int l, int r, Operation o, int i = -1) {
        if (!~i) i = nodes++;
        STT[i].family = {l, r, -1};
        STT[i].op = o;
        if (~l) STT[l].family[2] = i;
        if (~r) STT[r].family[2] = i;

        if (o == LeafVertex) STT[i].size = 1;
        else if (o == InnerVertex) STT[i].size = STT[l].size + 1;
        else if (o == Edge) STT[i].size = STT[l].size;
        else STT[i].size = STT[l].size + STT[r].size;

        push(i);
        return i;
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
                STT[i].path = leaf(i);
                return;
            }
            case InnerVertex: {
                STT[i].path = inner(i, STT[l].point);
                return;
            }
            case Edge: {
                STT[i].point = edge(STT[l].path);
                return;
            }
            case Rake: {
                STT[i].point = rake(STT[l].point, STT[r].point);
                return;
            }
            case Compress: {
                STT[i].path = compress(STT[l].path, STT[r].path);
                return;
            }
            default: return;
        }
    }

    long long query(int i) {
        auto val = max(STT[root].path.m.subpath, STT[root].path.aggregate);
        vals[i] = 0;
        for (; ~i; i = STT[i].family[2]) push(i);
        return val;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> values(n);
    for (int &vi : values) cin >> vi;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    vector<vector<int>> dag(n);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (int u : adj_list[v])
            if (u != prev) {
                dag[v].emplace_back(u);
                self(self, u, v);
            }
    };
    dfs(dfs);

    StaticTopTree stt(n, dag, values);
    pair<long long, int> p{0, -n};
    auto &[val, day] = p;
    for (int d = 0; d < n; d++) p = max(p, {stt.query(d), -d});
    cout << val << " " << -day;
}
