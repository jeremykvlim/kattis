#include <bits/stdc++.h>
using namespace std;

struct ImplicitTreap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        bool flip, dir;
        int id, size;

        TreapNode(int i = 0) : family{0, 0, 0}, prio(rng()), flip(false), dir(false), id(i), size(1) {}
    };

    vector<TreapNode> T;
    int root;

    ImplicitTreap(int n) : T(n + 1), root(0) {
        iota(T.begin(), T.end(), 0);

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

    void pull(int i) {
        if (!i) return;

        auto [l, r, p] = T[i].family;
        T[i].size = size(l) + size(r) + 1;
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = T[i].family;
        swap(l, r);
        T[i].flip = !T[i].flip;
        T[i].dir = !T[i].dir;
    }

    void push(int i) {
        if (!i) return;
        if (T[i].flip) {
            auto [l, r, p] = T[i].family;
            if (l) flip(l);
            if (r) flip(r);
            T[i].flip = false;
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

    void update(int l, int r) {
        auto [ll, lr] = split(root, l - 1);
        auto [rl, rr] = split(lr, r - l + 1);
        if (rl) flip(rl);
        root = meld(meld(ll, rl), rr);
    }

    vector<int> in_order_traversal() {
        vector<int> order;
        auto dfs = [&](auto &&self, int i) {
            if (!i) return;
            push(i);
            auto [l, r, p] = T[i].family;
            self(self, l);
            order.emplace_back(T[i].dir ? -T[i].id : T[i].id);
            self(self, r);
        };
        dfs(dfs, root);
        return order;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    ImplicitTreap treap(n);
    while (q--) {
        int i, j;
        cin >> i >> j;

        treap.update(i, j);
    }
    for (int k : treap.in_order_traversal()) cout << k << " ";
}
