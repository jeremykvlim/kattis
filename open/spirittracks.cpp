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
        int top;
        array<pair<int, int>, 2> interval;

        Path(int top = -1) : top(top), interval{} {}
    };

    struct Point {
        int weight;

        Point(int weight = -1) : weight(weight) {}
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
    vector<int> heavy, s, w;

    StaticTopTree(int n, vector<vector<int>> &adj_list, const vector<int> &s, const vector<int> &w) : root(-1), nodes(n), STT(4 * n), s(s), w(w) {
        heavy.resize(n, -1);
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
        Path path(i);
        path.interval = !i || !~heavy[i] ? array{make_pair(INT_MAX, 0), make_pair(INT_MAX, 0)} : array{make_pair(0, INT_MAX), make_pair(0, INT_MAX)};
        return path;
    }

    Path inner(int i, const Point &p) {
        Path path(i);
        for (int dir = 0; dir < 2; dir++) path.interval[dir] = w[!dir ? heavy[i] : i] < p.weight ? make_pair(0, s[i]) : make_pair(s[i], INT_MAX);
        return path;
    }

    Point edge(const Path &p) {
        return {w[p.top]};
    }

    Point rake(const Point &pl, const Point &pr) {
        return pl;
    }

    Path compress(const Path &pl, const Path &pr) {
        Path path(pl.top);
        for (int dir = 0; dir < 2; dir++) path.interval[dir] = {max(pl.interval[dir].first, pr.interval[dir].first), min(pl.interval[dir].second, pr.interval[dir].second)};
        return path;
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

    void update(int i, int value) {
        s[i] = value;
        for (; ~i; i = STT[i].family[2]) push(i);
    }

    int query(int i, int f) {
        auto contains = [&](int i, int dir) {
            auto [l, r] = STT[i].path.interval[dir];
            return l < f && f <= r;
        };

        auto resolve = [&](int i, int dir) {
            for (;;) {
                auto [l, r, p] = STT[i].family;
                if (STT[i].op == Compress) {
                    int j = dir ? r : l, k = dir ? l : r;
                    i = !contains(j, dir) ? j : k;
                } else if (STT[i].op == Edge) {
                    i = l;
                    dir = 0;
                } else if (STT[i].op == InnerVertex || STT[i].op == Rake) i = l;
                else return i;
            }
        };

        int side = !i;
        for (;;) {
            int p = STT[i].family[2];
            auto [l, r, _] = STT[p].family;

            if (STT[p].op == Compress) {
                if (i == l) {
                    if (!side) i = p;
                    else if (contains(r, 0)) {
                        i = p;
                        side = 1;
                    } else return resolve(r, 0);
                } else {
                    if (side == 1) i = p;
                    else if (contains(l, 1)) {
                        i = p;
                        side = 0;
                    } else return resolve(l, 1);
                }
            } else if (STT[p].op == Edge || STT[p].op == Rake) {
                i = p;
                side = 2;
            } else if (STT[p].op == InnerVertex) {
                i = p;
                side = f <= s[p] ? w[heavy[p]] < w[p] : w[heavy[p]] > w[p];
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<array<int, 3>> edges(n - 1);
    vector<int> degree(n, 0);
    for (auto &[u, v, p] : edges) {
        cin >> u >> v >> p;
        u--;
        v--;

        degree[u]++;
        degree[v]++;
    }

    vector<int> spirit(n);
    while (m--) {
        int j, s;
        cin >> j >> s;

        spirit[j - 1] = s;
    }

    int root = find(degree.begin(), degree.end(), 1) - degree.begin();
    swap(spirit[0], spirit[root]);
    auto id = [&](int v) {
        if (!v) return root;
        if (v == root) return 0;
        return v;
    };

    vector<vector<pair<int, int>>> adj_list_undirected(n);
    for (auto [u, v, w] : edges) {
        u = id(u);
        v = id(v);
        adj_list_undirected[u].emplace_back(v, w);
        adj_list_undirected[v].emplace_back(u, w);
    }

    vector<int> weight(n, -1);
    vector<vector<int>> adj_list(n);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (auto [u, w] : adj_list_undirected[v])
            if (u != prev) {
                weight[u] = w;
                adj_list[v].emplace_back(u);
                self(self, u, v);
            }
    };
    dfs(dfs);

    StaticTopTree stt(n, adj_list, spirit, weight);
    while (q--) {
        int type;
        cin >> type;

        if (type == 1) {
            int a, b;
            cin >> a >> b;

            stt.update(id(a - 1), b);
        } else {
            int d, f;
            cin >> d >> f;

            cout << id(stt.query(id(d - 1), f)) + 1 << "\n";
        }
    }
}
