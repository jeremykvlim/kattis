#include <bits/stdc++.h>
using namespace std;

struct PersistentSegmentTree {
    struct Segment {
        int sum;

        Segment(int s = 0) : sum(s) {}

        auto operator+=(const Segment &seg) {
            sum += seg.sum;
            return *this;
        }

        friend auto operator+(Segment sl, Segment sr) {
            return sl += sr;
        }

        friend auto operator+(Segment s, int v) {
            return Segment(s.sum + v);
        }
    };

    int root_l, root_r;
    vector<int> roots;
    vector<Segment> ST;
    vector<pair<int, int>> children;

    PersistentSegmentTree(int l, int r) : root_l(l), root_r(r), roots{0}, ST(1), children{{0, 0}} {}

    void modify(int v, int pos, int i) {
        roots.emplace_back(modify(v, pos, root_l, root_r, roots[i]));
    }

    int modify(int v, int pos, int tl, int tr, int i) {
        if (tr - tl == 1) {
            children.emplace_back(0, 0);
            ST.emplace_back(ST[i] + v);
            return ST.size() - 1;
        }

        int tm = tl + (tr - tl) / 2;
        auto [cl, cr] = children[i];
        if (pos < tm) cl = modify(v, pos, tl, tm, cl);
        else cr = modify(v, pos, tm, tr, cr);

        children.emplace_back(cl, cr);
        ST.emplace_back(ST[cl] + ST[cr]);
        return ST.size() - 1;
    }

    Segment query(int l, int r, int i) {
        return query(l, r, root_l, root_r, roots[i]);
    }

    Segment query(int l, int r, int tl, int tr, int i) {
        if (!i || r <= tl || tr <= l) return {};
        if (l <= tl && tr <= r) return ST[i];

        int tm = tl + (tr - tl) / 2;
        auto [cl, cr] = children[i];
        return query(l, r, tl, tm, cl) + query(l, r, tm, tr, cr);
    }
};

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip;
        int v1 = 0, v2 = 0, i1 = 0, i2 = 0;

        SplayNode(int v = 0, int i = -1) : family{0, 0, 0}, flip(false), v1(v), v2(v), i1(i), i2(i) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n) : ST(n) {}

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
        ST[i].v2 = ST[i].v1;
        ST[i].i2 = ST[i].i1;
        auto update = [&](int c) {
            if (ST[i].v2 < ST[c].v2) {
                ST[i].v2 = ST[c].v2;
                ST[i].i2 = ST[c].i2;
            }
        };
        auto [l, r, p] = ST[i].family;
        update(l);
        update(r);
    }

    void reverse(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip ^= 1;
    }

    void push(int i) {
        if (!i) return;
        if (ST[i].flip) {
            auto [l, r, p] = ST[i].family;
            if (l) reverse(l);
            if (r) reverse(r);
            ST[i].flip = false;
        }
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) {return ST[parent].family[1] == i;};

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
    LinkCutTree(int n) : SplayTree(n + 1) {}

    void access(int i) {
        for (int prev = 0, curr = i; curr; prev = curr, curr = ST[curr].family[2]) {
            splay(curr);
            ST[curr].family[1] = prev;
            pull(curr);
        }
        splay(i);
    }

    int find(int i) {
        access(i);
        while (ST[i].family[0]) push(i = ST[i].family[0]);
        return i;
    }

    void reroot(int i) {
        access(i);
        reverse(i);
    }

    void link(int i, int j) {
        reroot(i);
        ST[i].family[2] = j;
    }

    void cut(int i, int j) {
        reroot(j);
        access(i);
        ST[i].family[0] = ST[j].family[2] = 0;
        pull(i);
    }

    int query(int i, int j) {
        reroot(j);
        access(i);
        return ST[i].i2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<array<int, 3>> edges(m + 1);
        vector<int> weights(m + 1);
        for (int i = 1; i <= m; i++) {
            cin >> edges[i][0] >> edges[i][1] >> edges[i][2];

            weights[i] = edges[i][2];
        }
        sort(edges.begin() + 1, edges.end(), [](auto a1, auto a2) {return a1[2] > a2[2];});
        sort(weights.begin() + 1, weights.end());

        LinkCutTree lct(m + n);
        for (int i = 1; i <= m; i++) lct[i + n] = {edges[i][2], i};

        vector<int> offset(m + 1, 0);
        PersistentSegmentTree pst(1, m);
        for (int i = 1; i <= m; i++) {
            auto [xi, yi, wi] = edges[i];
            pst.modify(wi, m - i + 1, i - 1 + offset[i]);

            if (lct.find(xi) == lct.find(yi)) {
                int j = lct.query(xi, yi);

                auto [xj, yj, wj] = edges[j];
                lct.cut(j + n, xj);
                lct.cut(j + n, yj);
                pst.modify(-wj, m - j + 1, i + offset[i]++);
            }

            lct.link(i + n, xi);
            lct.link(i + n, yi);
            if (i < m) offset[i + 1] += offset[i];
        }

        int q;
        cin >> q;

        int c = 0;
        while (q--) {
            int l, h;
            cin >> l >> h;

            int r = 1 + (upper_bound(weights.begin() + 1, weights.end(), h - c) - weights.begin() - 1), 
                i = m - (lower_bound(weights.begin() + 1, weights.end(), l - c) - weights.begin() - 1);
            
            c = pst.query(1, r, i + offset[i]).sum;
            cout << c << "\n";
        }
    }
}
