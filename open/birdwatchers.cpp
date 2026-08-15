#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        int value = 0, maximum = 0;

        SplayNode(int v = 0) : family{0, 0, 0}, value(v), maximum(v) {}
    };

    vector<SplayNode> ST;
    vector<int> lazy;

    SplayTree(int n, const vector<int> &a, const vector<int> &p) : ST(n + 1), lazy(n + 1, 0) {
        for (int i = 1; i <= n; i++) {
            ST[i] = a[i];
            ST[i].family[2] = p[i];
        }
    }

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].maximum = max(ST[i].value, max(ST[l].maximum, ST[r].maximum));
    }

    void apply(int i, int v) {
        if (!i) return;
        ST[i].value += v;
        ST[i].maximum += v;
        lazy[i] += v;
    }

    void push(int i) {
        if (!i || !lazy[i]) return;
        auto [l, r, p] = ST[i].family;
        apply(l, lazy[i]);
        apply(r, lazy[i]);
        lazy[i] = 0;
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
        while (ST[i].family[0]) i = ST[i].family[0];
        return i;
    }
};

struct RootedLinkCutTree : SplayTree {
    RootedLinkCutTree(int n, const vector<int> &a, const vector<int> &p) : SplayTree(n, a, p) {}

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

    void link(int i, int j) {
        access(i);
        ST[i].family[2] = j;
    }

    void cut(int i) {
        access(i);
        auto &[l, r, p] = ST[i].family;
        l = ST[l].family[2] = 0;
        pull(i);
    }

    void update(int i, int j) {
        access(i);
        int v = ST[i].value;
        apply(ST[i].family[0], -v);
        cut(i);

        access(j);
        apply(j, v);
        link(i, j);
    }

    pair<int, int> query(int i, int k) {
        access(i);
        for (;;) {
            push(i);
            auto [l, r, p] = ST[i].family;
            if (r && ST[r].maximum >= k) i = r;
            else if (ST[i].value < k) i = l;
            else break;
        }
        splay(i);
        return {ST[i].value, i};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    int total = 0, t = -1;
    vector<int> s(n + 1), m(n + 1);
    vector<vector<int>> adj_list(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> s[i] >> m[i];

        total += m[i];
        if (!s[i]) t = i;
        else adj_list[s[i]].emplace_back(i);
    }

    auto dfs = [&](auto &&self, int v) -> void {
        for (int u : adj_list[v]) {
            self(self, u);
            m[v] += m[u];
        }
    };
    for (int v = 1; v <= n; v++)
        if (!s[v]) dfs(dfs, v);

    int k = (total + 1) / 2;
    for (int i = 1; i <= n; i++)
        if (m[i] >= k && m[t] > m[i]) t = i;

    RootedLinkCutTree lct(n, m, s);
    cout << t << "\n";
    while (q--) {
        int x, z;
        cin >> x >> z;
        x = 1 + (t + x) % n;
        z = 1 + (t + z) % n;

        lct.update(x, z);
        cout << (t = min(lct.query(z, k), lct.query(t, k)).second) << "\n";
    }
}
