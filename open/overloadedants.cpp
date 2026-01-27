#include <bits/stdc++.h>
using namespace std;

struct ImplicitTreap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        int size, count;
        bool alive, kill;
        long long s, sl, sr, lazy_x, lazy_y;

        TreapNode(int w = 0) : family{0, 0, 0}, prio(rng()), size(1), count(1),
                               alive(true), kill(false), s(w), sl(w), sr(w), lazy_x(0), lazy_y(0) {}
    };

    vector<TreapNode> T;
    int root;

    ImplicitTreap(int n, const vector<int> &w) : T(n + 1), root(0) {
        for (int i = 1; i <= n; i++) T[i] = w[i - 1];

        deque<int> st{0};
        for (int i = 1; i <= n + 1; i++) {
            int j = 0;
            for (; st.size() > 1 && (i == n + 1 || T[st.back()].prio >= T[i].prio); ) {
                j = st.back();
                st.pop_back();
            }

            if (i == n + 1) root = j;
            else {
                if (st.back()) {
                    T[i].family[2] = st.back();
                    T[st.back()].family[1] = i;
                }
                if (j) {
                    T[j].family[2] = i;
                    T[i].family[0] = j;
                }
                st.emplace_back(i);
            }
        }

        auto dfs = [&](auto &&self, int v) -> void {
            if (!v) return;
            auto [l, r, p] = T[v].family;
            self(self, l);
            self(self, r);
            pull(v);
        };
        dfs(dfs, root);
    }

    int size(int i) {
        return !i ? 0 : T[i].size;
    }

    int count(int i) {
        return !i ? 0 : T[i].count;
    }

    long long sl(int i) {
        return !i ? LLONG_MAX : T[i].sl;
    }

    long long sr(int i) {
        return !i ? LLONG_MIN : T[i].sr;
    }

    void pull(int i) {
        if (!i) return;

        auto [l, r, p] = T[i].family;
        T[i].size = size(l) + size(r) + 1;
        T[i].count = count(l) + count(r) + T[i].alive;
        T[i].sl = min({sl(l), sl(r), T[i].alive ? T[i].s : LLONG_MAX});
        T[i].sr = max({sr(l), sr(r), T[i].alive ? T[i].s : LLONG_MIN});
    }

    void kill(int i) {
        if (!i) return;

        T[i].alive = false;
        T[i].kill = true;
        T[i].count = T[i].lazy_x = T[i].lazy_y = 0;
        T[i].sl = LLONG_MAX;
        T[i].sr = LLONG_MIN;
    }

    void apply(int i, long long x, long long y) {
        if (!i || !T[i].count) return;

        T[i].lazy_x += x;
        T[i].lazy_y += y;
        T[i].sl -= x + y * (size(i) - 1);
        T[i].sr -= x;
    }

    void push(int i) {
        if (!i) return;

        if (T[i].kill) {
            auto [l, r, p] = T[i].family;
            if (l) kill(l);
            if (r) kill(r);
            T[i].kill = false;
        }

        if (T[i].lazy_x || T[i].lazy_y) {
            auto [l, r, p] = T[i].family;
            if (l) apply(l, T[i].lazy_x, T[i].lazy_y);
            if (T[i].alive) T[i].s -= T[i].lazy_x + T[i].lazy_y * size(l);
            if (r) apply(r, T[i].lazy_x + T[i].lazy_y * (size(l) + 1), T[i].lazy_y);
            T[i].lazy_x = T[i].lazy_y = 0;
        }
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};

        push(i);
        auto [l, r, p] = T[i].family;
        int sl = size(l);
        if (k <= sl) {
            auto [ll, lr] = split(l, k);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = split(r, k - sl - 1);
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        }
    }

    int meld(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }

        if (T[i].prio < T[j].prio) {
            push(i);
            attach(i, 1, meld(T[i].family[1], j));
            T[i].family[2] = 0;
            return i;
        } else {
            push(j);
            attach(j, 0, meld(i, T[j].family[0]));
            T[j].family[2] = 0;
            return j;
        }
    }

    void rotate(int l, int r, int x) {
        if (!x) return;

        auto [ll, lr] = split(root, l - 1);
        auto [rl, rr] = split(lr, r - l + 1);
        auto [ml, mr] = split(rl, x);
        rl = meld(mr, ml);
        root = meld(ll, meld(rl, rr));
    }

    long long carry(int l, int r, int x, int y) {
        auto [ll, lr] = split(root, l - 1);
        auto [rl, rr] = split(lr, r - l + 1);
        auto c = query(rl, x, y);
        root = meld(ll, meld(rl, rr));
        return c;
    }

    long long query(int i, long long x, long long y) {
        if (!i || !T[i].count) return 0;

        if (T[i].sl >= x + y * (size(i) - 1)) {
            apply(i, x, y);
            return 0;
        }

        if (T[i].sr < x) {
            int c = T[i].count;
            kill(i);
            return c;
        }

        push(i);
        auto [l, r, p] = T[i].family;
        auto c = query(l, x, y);
        if (T[i].alive) {
            T[i].s -= x + y * size(l);
            if (T[i].s < 0) {
                T[i].alive = false;
                c++;
            }
        }
        c += query(r, x + y * (size(l) + 1), y);
        pull(i);
        return c;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> w(n);
    for (int &wi : w) cin >> wi;

    ImplicitTreap treap(n, w);
    while (m--) {
        int q, l, r, x;
        cin >> q >> l >> r >> x;

        if (l > r) swap(l, r);
        int len = r - l + 1;
        if (q == 1) {
            if (len < 2) continue;

            x %= len;
            if (x < 0) x += len;
            treap.rotate(l, r, x);
        } else {
            int y;
            cin >> y;
            cout << treap.carry(l, r, x, y) << "\n";
        }
    }
}
