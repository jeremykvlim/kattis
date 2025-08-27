#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        int size, count;
        bool alive, kill;
        long long s, sl, sr, lazy_x, lazy_y;

        SplayNode(int w = 0) : family{0, 0, 0}, size(1), count(1),
                               alive(true), kill(false), s(w), sl(w), sr(w), lazy_x(0), lazy_y(0) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n, const vector<int> &w) : ST(n + 1) {
        for (int i = 1; i <= n; i++) ST[i] = w[i - 1];
        for (int i = 1; i < n; i++) {
            ST[i].family[1] = i + 1;
            ST[i + 1].family[2] = i;
            pull(i);
        }
        pull(n);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    int size(int i) {
        return !i ? 0 : ST[i].size;
    }

    int count(int i) {
        return !i ? 0 : ST[i].count;
    }

    long long sl(int i) {
        return !i ? LLONG_MAX : ST[i].sl;
    }

    long long sr(int i) {
        return !i ? LLONG_MIN : ST[i].sr;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = ST[i].family;
        ST[i].size = size(l) + size(r) + 1;
        ST[i].count = count(l) + count(r) + ST[i].alive;
        ST[i].sl = min({sl(l), sl(r), ST[i].alive ? ST[i].s : LLONG_MAX});
        ST[i].sr = max({sr(l), sr(r), ST[i].alive ? ST[i].s : LLONG_MIN});
    }

    void kill(int i) {
        if (!i) return;
        ST[i].alive = false;
        ST[i].kill = true;
        ST[i].count = ST[i].lazy_x = ST[i].lazy_y = 0;
        ST[i].sl = LLONG_MAX;
        ST[i].sr = LLONG_MIN;
    }

    void apply(int i, long long x, long long y) {
        if (!i || !ST[i].count) return;
        ST[i].lazy_x += x;
        ST[i].lazy_y += y;
        ST[i].sl -= x + y * (size(i) - 1);
        ST[i].sr -= x;
    }

    void push(int i) {
        if (!i) return;

        if (ST[i].kill) {
            auto [l, r, p] = ST[i].family;
            if (l) kill(l);
            if (r) kill(r);
            ST[i].kill = false;
        }

        if (ST[i].lazy_x || ST[i].lazy_y) {
            auto [l, r, p] = ST[i].family;
            if (l) apply(l, ST[i].lazy_x, ST[i].lazy_y);
            if (ST[i].alive) ST[i].s -= ST[i].lazy_x + ST[i].lazy_y * size(l);
            if (r) apply(r, ST[i].lazy_x + ST[i].lazy_y * (size(l) + 1), ST[i].lazy_y);
            ST[i].lazy_x = ST[i].lazy_y = 0;
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

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};
        for (;;) {
            push(i);
            auto [l, r, p] = ST[i].family;
            int sl = size(l);
            if (k <= sl) {
                if (!l) {
                    splay(i);
                    return {0, i};
                }
                i = l;
            } else if (k == sl + 1) {
                splay(i);
                r = ST[i].family[1];
                ST[i].family[1] = 0;
                pull(i);
                if (r) ST[r].family[2] = 0;
                return {i, r};
            } else {
                k -= sl + 1;
                if (!r) {
                    splay(i);
                    return {i, 0};
                }
                i = r;
            }
        }
    }

    int subtree_max(int i) {
        while (ST[i].family[1]) {
            push(i);
            i = ST[i].family[1];
        }
        push(i);
        return i;
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;
        i = subtree_max(i);
        splay(i);
        ST[i].family[1] = j;
        ST[j].family[2] = i;
        pull(i);
        return i;
    }

    long long query(int i, long long x, long long y) {
        if (!i || !ST[i].count) return 0;

        if (ST[i].sl >= x + y * (size(i) - 1)) {
            apply(i, x, y);
            return 0;
        }

        if (ST[i].sr < x) {
            int c = ST[i].count;
            kill(i);
            return c;
        }

        push(i);
        auto [l, r, p] = ST[i].family;
        auto c = query(l, x, y);
        if (ST[i].alive) {
            ST[i].s -= x + y * size(l);
            if (ST[i].s < 0) {
                ST[i].alive = false;
                c++;
            }
        }
        c += query(r, x + y * (size(l) + 1), y);
        pull(i);
        return c;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> w(n);
    for (int &wi : w) cin >> wi;

    SplayTree st(n, w);
    int root = 1;
    while (m--) {
        int q, l, r, x;
        cin >> q >> l >> r >> x;

        if (l > r) swap(l, r);
        int len = r - l + 1;
        if (q == 1) {
            if (len < 2) continue;

            x %= len;
            if (x < 0) x += len;
            if (!x) continue;

            auto [ll, lr] = st.split(root, l - 1);
            auto [rl, rr] = st.split(lr, len);
            auto [ml, mr] = st.split(rl, x);
            rl = st.meld(mr, ml);
            root = st.meld(ll, st.meld(rl, rr));
        } else {
            int y;
            cin >> y;

            auto [ll, lr] = st.split(root, l - 1);
            auto [rl, rr] = st.split(lr, len);
            cout << st.query(rl, x, y) << "\n";
            root = st.meld(ll, st.meld(rl, rr));
        }
    }
}
