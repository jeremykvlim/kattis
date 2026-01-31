#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        int size;
        unsigned long long prio;
        int key;

        TreapNode(int k = 0) : family{0, 0, 0}, size(1), prio(rng()), key(k) {}
    };

    vector<TreapNode> T;

    Treap(int n) : T(n + 1) {
        iota(T.begin(), T.end(), 0);
    }

    int size(int i) const {
        return !i ? 0 : T[i].size;
    }

    int pull(int i) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        T[i].size = size(l) + size(r) + 1;
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, const int &key) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        if (T[i].key > key) {
            auto [ll, lr] = split(l, key);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = split(r, key);
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        }
    }

    pair<int, int> implicit_split(int i, int k) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        int sl = size(l);
        if (k <= sl) {
            auto [ll, lr] = implicit_split(l, k);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = implicit_split(r, k - sl - 1);
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        }
    }

    int meld_by_key(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            auto [jl, jr] = split(j, T[i].key);
            attach(i, 0, meld_by_key(T[i].family[0], jl));
            attach(i, 1, meld_by_key(T[i].family[1], jr));
            T[i].family[2] = 0;
            return i;
        } else {
            auto [il, ir] = split(i, T[j].key);
            attach(j, 0, meld_by_key(T[j].family[0], il));
            attach(j, 1, meld_by_key(T[j].family[1], ir));
            T[j].family[2] = 0;
            return j;
        }
    }

    int find_by_order(int i, int k) const {
        for (; i;) {
            int sl = size(T[i].family[0]);
            if (k < sl) i = T[i].family[0];
            else if (k == sl) return i;
            else {
                k -= sl + 1;
                i = T[i].family[1];
            }
        }
        return 0;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    stack<int> s;
    Treap treap(n);
    vector<int> root(n + 1);
    iota(root.begin(), root.end(), 0);
    while (q--) {
        int t, i, j;
        cin >> t >> i >> j;

        if (t == 1) cout << treap[treap.find_by_order(root[i], j - 1)].key << "\n";
        else if (t == 2) {
            root[i] = treap.meld_by_key(root[i], root[j]);
            s.emplace(j);
        } else {
            auto [l, r] = treap.implicit_split(root[i], j);
            root[i] = l;
            root[s.top()] = r;
            s.pop();
        }
    }
}
