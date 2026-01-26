#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        int l, r, size;
        unsigned long long prio;
        int key;

        TreapNode() : l(0), r(0), size(1), prio(0), key(0) {}

        auto & operator=(const int &k) {
            prio = rng();
            key = k;
            return *this;
        }
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
        T[i].size = size(T[i].l) + size(T[i].r) + 1;
        return i;
    }

    pair<int, int> split(int i, const int &key) {
        if (!i) return {0, 0};

        if (T[i].key <= key) {
            auto [l, r] = split(T[i].r, key);
            T[i].r = l;
            return {pull(i), r};
        } else {
            auto [l, r] = split(T[i].l, key);
            T[i].l = r;
            return {l, pull(i)};
        }
    }

    pair<int, int> split_by_order(int i, int k) {
        if (!i) return {0, 0};

        int sl = size(T[i].l);
        if (k <= sl) {
            auto [l, r] = split_by_order(T[i].l, k);
            T[i].l = r;
            return {l, pull(i)};
        } else {
            auto [l, r] = split_by_order(T[i].r, k - sl - 1);
            T[i].r = l;
            return {pull(i), r};
        }
    }

    int meld_by_key(int i, int j) {
        if (!i || !j) return i ^ j;

        if (T[i].prio > T[j].prio) {
            auto [jl, jr] = split(j, T[i].key);
            T[i].l = meld_by_key(T[i].l, jl);
            T[i].r = meld_by_key(T[i].r, jr);
            return pull(i);
        } else {
            auto [il, ir] = split(i, T[j].key);
            T[j].l = meld_by_key(T[j].l, il);
            T[j].r = meld_by_key(T[j].r, ir);
            return pull(j);
        }
    }

    int find_by_order(int i, int k) const {
        for (; i;) {
            int sl = size(T[i].l);
            if (k < sl) i = T[i].l;
            else if (k == sl) return i;
            else {
                k -= sl + 1;
                i = T[i].r;
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
            auto [l, r] = treap.split_by_order(root[i], j);
            root[i] = l;
            root[s.top()] = r;
            s.pop();
        }
    }
}
