#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip, used;
        int base, aggregate, count;

        SplayNode() : family{0, 0, 0}, flip(false), used(false), base(-1), aggregate(-1), count(0) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n, int m, const auto &edges) : ST(n + m + 1) {
        for (int i = 1; i <= n; i++) {
            ST[i].base = ST[i].aggregate = -1e9;
            ST[i].used = true;
        }
        for (auto [w, i, u, v] : edges) ST[i].base = ST[i].aggregate = w;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].aggregate = ST[i].base;
        ST[i].count = !ST[i].used;
        if (ST[i].aggregate < ST[l].aggregate) {
            ST[i].aggregate = ST[l].aggregate;
            ST[i].count = ST[l].count;
        } else if (ST[i].aggregate == ST[l].aggregate) ST[i].count += ST[l].count;

        if (ST[i].aggregate < ST[r].aggregate) {
            ST[i].aggregate = ST[r].aggregate;
            ST[i].count = ST[r].count;
        } else if (ST[i].aggregate == ST[r].aggregate) ST[i].count += ST[r].count;
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip ^= true;
    }

    void push(int i) {
        if (!i) return;
        if (ST[i].flip) {
            auto [l, r, p] = ST[i].family;
            if (l) flip(l);
            if (r) flip(r);
            ST[i].flip = false;
        }
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
    LinkCutTree(int n, int m, const auto &edges) : SplayTree(n, m, edges) {}

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

    int count_path_max(int i) {
        if (!ST[i].count) return 0;
        int count = 0;
        auto dfs = [&](auto &&self, int v) -> void {
            if (!v) return;
            auto [l, r, p] = ST[v].family;
            if (l && ST[l].aggregate == ST[i].aggregate && ST[l].count) self(self, l);
            if (r && ST[r].aggregate == ST[i].aggregate && ST[r].count) self(self, r);
            if (!ST[v].used && ST[v].base == ST[i].aggregate) {
                ST[v].used = true;
                count++;
            }
            pull(v);
        };
        dfs(dfs, i);
        return count;
    }

    int query(int i, int j, int w) {
        split(i, j);
        if (ST[i].aggregate < w) return 1;
        return count_path_max(i) + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 4>> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i][2] >> edges[i][3] >> edges[i][0];

        edges[i][1] = i + n + 1;
    }
    sort(edges.begin(), edges.end());

    LinkCutTree lct(n, m, edges);
    int heavy = 0;
    for (auto [w, i, u, v] : edges)
        if (lct.find(u) != lct.find(v)) {
            lct.link(u, i);
            lct.link(i, v);
        } else heavy += lct.query(u, v, w);
    cout << heavy;
}
