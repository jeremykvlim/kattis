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
        bool skip;

        Path(bool skip = false) : skip(skip) {}
    };

    struct Point {
        int largest;

        Point(int largest = 0) : largest(largest) {}
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

    StaticTopTree(int n, vector<vector<int>> &adj_list) : root(-1), nodes(n), STT(4 * n) {
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

    Path leaf() {
        return {true};
    }

    Path inner() {
        return {false};
    }

    Point edge(int size) {
        return {size};
    }

    Point rake(const Point &pl, const Point &pr) {
        return {max(pl.largest, pr.largest)};
    }

    Path compress(const Path &pl, const Path &pr) {
        return {pl.skip && pr.skip};
    }

    void push(int i) {
        auto [l, r, p] = STT[i].family;
        switch (STT[i].op) {
            case LeafVertex: {
                STT[i].path = leaf();
                return;
            }
            case InnerVertex: {
                STT[i].path = inner();
                return;
            }
            case Edge: {
                STT[i].point = edge(STT[l].size);
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

    pair<int, int> process(int i, int value, int rem = 0) {
        if (!~i) return {0, 0};

        auto [l, r, p] = STT[i].family;
        if (STT[i].op == Edge || STT[i].op == Rake) {
            if (STT[i].point.largest <= value) return {0, STT[i].size};
            if (STT[i].op == Edge) return process(l, value);
            auto [quo_l, rem_l] = process(l, value);
            auto [quo_r, rem_r] = process(r, value);
            return {quo_l + quo_r, rem_l + rem_r};
        }

        if (rem + STT[i].size <= value) return {0, rem + STT[i].size};

        if (STT[i].path.skip) return {(rem + STT[i].size) / (value + 1), (rem + STT[i].size) % (value + 1)};

        if (STT[i].op == Compress) {
            auto [quo_r, rem_r] = process(r, value, rem);
            auto [quo_l, rem_l] = process(l, value, rem_r);
            return {quo_l + quo_r, rem_l};
        }

        auto [quo_l, rem_l] = process(l, value);
        rem += rem_l + 1;
        if (rem <= value) return {quo_l, rem};
        return {quo_l + 1, 0};
    }

    int query(int value) {
        return process(root, value, 0).first;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n, -1);
    vector<vector<int>> adj_list(n);
    for (int i = 1; i < n; i++) {
        cin >> a[i];
        a[i]--;

        adj_list[a[i]].emplace_back(i);
    }

    StaticTopTree stt(n, adj_list);
    vector<int> count(n, -1), damage(n, 0);
    auto dnc = [&](auto &&self, int cl, int cr, int l, int r) -> void {
        if (cl > cr) return;

        if (l == r) {
            for (int i = cl; i <= cr; i++) damage[i] = l;
            return;
        }

        int m = l + (r - l) / 2;
        if (!~count[m]) count[m] = stt.query(m);
        self(self, cl, min(cr, count[m] - 1), m + 1, r);
        self(self, max(cl, count[m]), cr, l, m);
    };
    dnc(dnc, 1, n - 1, 1, n - 1);
    for (int i = 1; i < n; i++) cout << damage[i] << " ";
}
