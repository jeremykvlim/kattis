#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        long long value = 0, sum = 0;

        SplayNode(long long v = 0) : family{0, 0, 0}, value(v), sum(v) {}

        auto operator=(long long v) {
            value = sum = v;
        }
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n + 1) {}

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].sum = ST[i].value + ST[l].sum + ST[r].sum;
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
        pull(i);
    }

    long long path_sum() {
        access(1);
        return ST[1].sum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    long long m, s1, s2;
    cin >> Q >> m >> s1 >> s2;
    long long both = s1 + s2;

    RootedLinkCutTree lct(4 * Q);
    unordered_map<long long, map<long long, int>> nodes;
    vector<long long> haze(4 * Q, 0);
    vector<int> parent(4 * Q, 0);
    vector<pair<long long, long long>> cycles(4 * Q, {0, 0});
    auto unite = [&](int i, int j) {
        auto time = [&](int i) -> pair<int, long long> {
            auto [r, hits] = cycles[i];
            int k = hits & 1;
            if (k) r -= s1;
            return {k, r + (hits + k) / 2 * both};
        };
        auto [k, t1] = time(i);
        auto [_, t2] = time(j);

        auto dist = (j == 2 ? m : t2) - t1 - haze[i];
        lct[i] = dist <= (k ? s1 : s2) ? 1 : min((dist + both - 1) / both * 2, (dist + both - 1 - (k ? s1 : s2)) / both * 2 + 1);
        if (parent[i]) lct.cut(i);
        parent[i] = j;
        lct.link(i, j);
    };
    nodes[0][0] = 1;
    unite(1, 2);

    int count = 3;
    auto node = [&](int k, long long p) {
        if (p >= m) return 2;

        if (k) p += s1;
        auto r = p % both, hits = p / both * 2 - k;

        auto it1 = nodes[r].lower_bound(hits);
        if (it1 != nodes[r].end() && it1->first == hits) return it1->second;

        int i = count++;
        it1 = nodes[r].emplace_hint(it1, hits, i);
        cycles[i] = {r, hits};

        auto it2 = next(it1);
        int j = it2 == nodes[r].end() ? 2 : it2->second;
        unite(i, j);

        int h = it1 == nodes[r].begin() ? 0 : prev(it1)->second;
        if (h && !haze[h]) unite(h, i);
        return i;
    };

    while (Q--) {
        int q;
        long long p;
        cin >> q >> p;

        if (q == 1) {
            long long h;
            cin >> h;

            for (int k = 0; k < 2; k++) {
                int i = node(k, p), j = node(k ^ 1, p + h + (k ? s1 : s2));
                haze[i] = h;
                unite(i, j);
            }
        } else
            for (int k = 0; k < 2; k++) {
                int i = node(k, p);
                haze[i] = 0;
                auto [r, hits] = cycles[i];
                auto it = nodes[r].upper_bound(hits);
                int j = it == nodes[r].end() ? 2 : it->second;
                unite(i, j);
            }

        cout << lct.path_sum() << "\n" << flush;
    }
}
