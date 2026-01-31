#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        pair<long long, int> key;
        long long val;

        TreapNode(const pair<long long, int> &k = {0, 0}) : family{0, 0, 0}, prio(rng()), key(k), val(0) {}
    };

    int root, nodes;
    vector<TreapNode> T;
    stack<int> recycled;
    vector<long long> lazy_key, lazy_val;

    Treap(int n) : root(0), nodes(1), T(n + 1), lazy_key(n + 1, 0), lazy_val(n + 1, 0) {}

    int node(const pair<long long, int> &key) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else i = nodes++;
        T[i] = key;
        return i;
    }

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
            apply(T[i].family[0], lazy_key[i], lazy_val[i]);
            apply(T[i].family[1], lazy_key[i], lazy_val[i]);
            lazy_key[i] = lazy_val[i] = 0;
        }
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, const pair<long long, int> &key) {
        if (!i) return {0, 0};
        push(i);
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

    int meld(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            attach(i, 1, meld(T[i].family[1], j));
            T[i].family[2] = 0;
            return i;
        } else {
            attach(j, 0, meld(i, T[j].family[0]));
            T[j].family[2] = 0;
            return j;
        }
    }

    int meld_by_key(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            push(i);
            auto [jl, jr] = split(j, T[i].key);
            attach(i, 0, meld_by_key(T[i].family[0], jl));
            attach(i, 1, meld_by_key(T[i].family[1], jr));
            T[i].family[2] = 0;
            return i;
        } else {
            push(j);
            auto [il, ir] = split(i, T[j].key);
            attach(j, 0, meld_by_key(T[j].family[0], il));
            attach(j, 1, meld_by_key(T[j].family[1], ir));
            T[j].family[2] = 0;
            return j;
        }
    }

    int insert(const pair<long long, int> &key) {
        int i = node(key);
        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        T[root].family[2] = 0;
        return i;
    }

    int erase(const pair<long long, int> &key) {
        root = erase(root, key);
        T[root].family[2] = 0;
        return root;
    }

    int erase(int i, const pair<long long, int> &key) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        if (T[i].key == key) {
            int m = meld(l, r);
            if (m) T[m].family[2] = 0;
            T[i] = {};
            lazy_key[i] = lazy_val[i] = 0;
            recycled.emplace(i);
            return m;
        }

        if (T[i].key > key) attach(i, 0, erase(l, key));
        else attach(i, 1, erase(r, key));
        return i;
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
            self(self, T[i].family[0]);
            order[T[i].key.second] = T[i].val;
            self(self, T[i].family[1]);
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
