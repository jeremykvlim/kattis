#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        int l, r;
        unsigned long long prio;
        pair<long long, int> key;
        long long val;

        TreapNode() : l(0), r(0), prio(0), key{0, 0}, val(0) {}

        auto & operator=(const pair<long long, int> &k) {
            prio = rng();
            key = k;
            return *this;
        }
    };

    int root, nodes;
    vector<TreapNode> T;
    vector<long long> lazy_key, lazy_val;

    Treap(int n) : root(0), nodes(1), T(n + 1), lazy_key(n + 1, 0), lazy_val(n + 1, 0) {}

    int pull(int i) {
        return i;
    }

    void apply(int i, const long long &key, const long long &val) {
        if (!i) return;

        T[i].key.first += key;
        T[i].val += val;
        lazy_key[i] += key;
        lazy_val[i] += val;
    }

    void push(int i) {
        if (!i) return;

        if (lazy_key[i] || lazy_val[i]) {
            apply(T[i].l, lazy_key[i], lazy_val[i]);
            apply(T[i].r, lazy_key[i], lazy_val[i]);
            lazy_key[i] = lazy_val[i] = 0;
        }
    }

    pair<int, int> split(int i, const pair<long long, int> &key) {
        if (!i) return {0, 0};

        push(i);
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

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;

        if (T[i].prio > T[j].prio) {
            push(i);
            T[i].r = meld(T[i].r, j);
            return pull(i);
        } else {
            push(j);
            T[j].l = meld(i, T[j].l);
            return pull(j);
        }
    }

    int meld_by_key(int i, int j) {
        if (!i || !j) return i ^ j;

        if (T[i].prio > T[j].prio) {
            push(i);
            auto [l, r] = split(j, T[i].key);
            T[i].l = meld_by_key(T[i].l, l);
            T[i].r = meld_by_key(T[i].r, r);
            return pull(i);
        } else {
            push(j);
            auto [l, r] = split(i, T[j].key);
            T[j].l = meld_by_key(T[j].l, l);
            T[j].r = meld_by_key(T[j].r, r);
            return pull(j);
        }
    }

    int insert(const pair<long long, int> &key) {
        int i = nodes++;
        T[i] = key;

        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        return i;
    }

    int erase(const pair<long long, int> &key) {
        return root = erase(root, key);
    }

    int erase(int i, const pair<long long, int> &key) {
        if (!i) return 0;
        if (T[i].key == key) return meld(T[i].l, T[i].r);

        if (T[i].key > key) T[i].l = erase(T[i].l, key);
        else T[i].r = erase(T[i].r, key);

        return pull(i);
    }

    void update(int c, int s) {
        auto [l, r] = split(root, {c, -1});
        if (r) apply(r, -c, s);
        root = meld_by_key(l, r);
    }

    vector<long long> in_order_traversal(int n) {
        vector<long long> order(n);
        auto dfs = [&](auto &&self, int i) {
            if (!i) return;

            push(i);
            self(self, T[i].l);
            order[T[i].key.second] = T[i].val;
            self(self, T[i].r);
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

    int n, C;
    cin >> n >> C;

    vector<int> s(n), c(n);
    for (int &si : s) cin >> si;
    for (int &ci : c) cin >> ci;

    Treap t(n);
    for (int i = 0; i < n; i++) {
        t.insert({C, i});
        t.update(c[i], s[i]);
    }

    for (auto v : t.in_order_traversal(n)) cout << v << " ";
}
