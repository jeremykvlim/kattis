#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip;
        long long value = 0, sum = 0;

        SplayNode(long long v = 0) : family{0, 0, 0}, flip(false), value(v), sum(v) {}

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

    void reroot(int i) {
        access(i);
        flip(i);
        pull(i);
    }

    void link(int i, int j) {
        reroot(i);
        ST[i].family[2] = j;
    }

    void cut(int i) {
        access(i);
        ST[i].family[0] = ST[ST[i].family[0]].family[2] = 0;
        pull(i);
    }

    void cut(int i, int j) {
        reroot(i);
        cut(j);
    }

    long long path_sum(int i, int j) {
        reroot(i);
        access(j);
        return ST[j].sum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    long long m, s1, s2;
    cin >> Q >> m >> s1 >> s2;
    long long both = s1 + s2;

    LinkCutTree lct(4 * Q);
    unordered_map<long long, map<long long, int>> nodes;
    vector<long long> haze(4 * Q, 0);
    vector<int> link(4 * Q, 0);
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
        if (link[i]) lct.cut(i, link[i]);
        link[i] = j;
        lct.link(i, j);
    };
    nodes[0][0] = 1;
    unite(1, 2);

    int count = 3;
    auto node = [&](int k, long long p) {
        if (p >= m) return 2;

        if (k) p += s1;
        auto r = p % both, hits = p / both * 2 - k;
        if (nodes[r].count(hits)) return nodes[r][hits];

        int i = count++;
        nodes[r][hits] = i;
        cycles[i] = {r, hits};

        auto it1 = nodes[r].upper_bound(hits);
        int j = it1 == nodes[r].end() ? 2 : it1->second;
        unite(i, j);

        auto it2 = nodes[r].lower_bound(hits);
        int h = it2 == nodes[r].begin() ? 0 : prev(it2)->second;
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

        cout << lct.path_sum(1, 2) << "\n" << flush;
    }
}
