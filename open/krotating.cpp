#include <bits/stdc++.h>
using namespace std;

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

    void pull(int i) {
        if (!i) return;
    }

    void flip(int i) {
        if (!i) return;
    }

    void push(int i) {
        if (!i) return;
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
            pull(j);
        };

        auto propagate = [&](auto &&self, int i) -> void {
            if (!root(i)) self(self, ST[i].family[2]);
            push(i);
        };

        propagate(propagate, i);
        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
        pull(i);
    }

    int subtree_min(int i) {
        while (ST[i].family[0]) {
            push(i);
            i = ST[i].family[0];
        }
        push(i);
        return i;
    }
};

struct LinkCutTree : SplayTree {
    LinkCutTree(int n) : SplayTree(n) {}

    void access(int i) {
        for (int u = 0, v = i; v; u = v, v = ST[v].family[2]) {
            splay(v);
            ST[v].family[1] = u;
            pull(v);
        }
        splay(i);
    }

    int find(int i) {
        access(i);
        i = subtree_min(i);
        splay(i);
        return i;
    }

    void reroot(int i) {
        access(i);
        flip(i);
        pull(i);
    }

    void link(int i, int j) {
        reroot(i);
        ST[i].family[2] = j;
    }

    void split(int i, int j) {
        reroot(j);
        access(i);
    }

    void cut(int i, int j) {
        split(i, j);
        ST[i].family[0] = ST[j].family[2] = 0;
        pull(i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<int> k(m + 1);
    vector<array<int, 10>> p(m + 1);
    vector<array<int, 3>> queries(q);
    vector<vector<int>> weeks(n + 1);
    for (auto &[b, t, x] : queries) {
        cin >> b >> t >> x;

        if (!b) {
            k[x] = t;
            for (int i = 0; i < t; i++) {
                cin >> p[x][i];

                weeks[p[x][i]].emplace_back(x);
            }
        }
    }

    int nodes = n;
    vector<vector<int>> id(n + 1);
    vector<array<pair<int, int>, 10>> edges(m + 1);
    for (int pi = 1; pi <= n; pi++)
        if (!weeks[pi].empty()) {
            sort(weeks[pi].begin(), weeks[pi].end());
            int w = weeks[pi].size();
            id[pi].resize(w);
            for (int j = 0; j < w; j++) id[pi][j] = ++nodes;

            for (int j = 0; j < w;) {
                int x = weeks[pi][j], u = !j ? pi : id[pi][j - 1];
                for (; j < w && x == weeks[pi][j]; j++);
                int v = id[pi][j - 1];
                for (int i = 0; i < k[x]; i++)
                    if (p[x][i] == pi) edges[x][i] = {u, v};
            }
        }

    LinkCutTree lct(nodes);
    vector<int> parent(nodes + 1, 0);
    for (int pi = 1; pi <= n; pi++)
        for (int j = 0; j < id[pi].size(); j++) {
            int v = id[pi][j];
            lct.link(v, parent[v] = !j ? pi : id[pi][j - 1]);
        }

    for (auto [b, t, x] : queries)
        if (!b) {
            for (int i = 0; i < k[x]; i++) {
                int u = edges[x][(i + 1) % k[x]].first, v = edges[x][i].second;
                if (parent[v]) lct.cut(v, parent[v]);
                if (u) lct.link(v, parent[v] = u);
            }
        } else {
            int j = upper_bound(weeks[t].begin(), weeks[t].end(), x) - weeks[t].begin();
            cout << lct.find(!j ? t : id[t][j - 1]) << "\n";
        }
}
