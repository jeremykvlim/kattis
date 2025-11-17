#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        int l, r, size;
        unsigned long long prio;
        int key, val;

        TreapNode() : l(0), r(0), size(1), prio(0), key(0), val(0) {}

        auto & operator=(const pair<int, int> &kv) {
            prio = rng();
            key = kv.first;
            val = kv.second;
            return *this;
        }
    };

    int root, nodes;
    vector<TreapNode> T;
    vector<int> lazy_key, lazy_val;

    Treap() : root(0), nodes(1), T(1), lazy_key(1, 0), lazy_val(1, 0) {}
    Treap(int n) : root(0), nodes(1), T(n + 1), lazy_key(n + 1, 0), lazy_val(n + 1, 0) {}

    int get(int i) const {
        return T[i].val;
    }

    void set(int i, int val) {
        T[i].val = val;
    }

    int size() const {
        return size(root);
    }

    int size(int i) const {
        return !i ? 0 : T[i].size;
    }

    int node(const int &key, const int &val) {
        int i = nodes++;
        if (nodes >= T.size()) {
            int s = nodes << 1;
            T.resize(s);
            lazy_key.resize(s);
            lazy_val.resize(s);
        }
        T[i] = {key, val};
        return i;
    }

    int pull(int i) {
        if (!i) return 0;

        T[i].size = size(T[i].l) + size(T[i].r) + 1;
        return i;
    }

    void apply(int i, const int &key, const int &val) {
        if (!i) return;

        T[i].key += key;
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

    pair<int, int> split(int i, const int &key) {
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

    int erase(const int &i) {
        if (!i) return 0;
        return root = erase(root, T[i].key);
    }

    int erase(int i, const int &key) {
        if (!i) return 0;
        if (T[i].key == key) return meld(T[i].l, T[i].r);

        if (T[i].key > key) T[i].l = erase(T[i].l, key);
        else T[i].r = erase(T[i].r, key);

        return pull(i);
    }

    int find(const int &key) const {
        for (int i = root; i; i = key < T[i].key ? T[i].l : T[i].r)
            if (key == T[i].key) return i;
        return 0;
    }

    int lower_bound(const int &key) const {
        int k = 0;
        for (int i = root; i;)
            if (T[i].key >= key) {
                k = i;
                i = T[i].l;
            } else i = T[i].r;
        return k;
    }

    int upper_bound(const int &key) const {
        int k = 0;
        for (int i = root; i;)
            if (T[i].key > key) {
                k = i;
                i = T[i].l;
            } else i = T[i].r;
        return k;
    }

    int front() const {
        if (!root) return 0;
        int i = root;
        for (; T[i].l; i = T[i].l);
        return i;
    }

    int back() const {
        if (!root) return 0;
        int i = root;
        for (; T[i].r; i = T[i].r);
        return i;
    }

    int insert(const int &key, const int &val) {
        int i = find(key);
        if (i) return i;

        i = node(key, val);
        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        return i;
    }

    int rank(int i) const {
        if (!i) return 0;
        return order_of_key(T[i].key);
    }

    int order_of_key(const int &key) const {
        int rank = 0;
        for (int i = root; i;)
            if (key <= T[i].key) i = T[i].l;
            else {
                rank += size(T[i].l) + 1;
                i = T[i].r;
            }
        return rank;
    }

    int find_by_order(int k) const {
        for (int i = root; i;) {
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

    int predecessor(const int &i) const {
        if (!i) return 0;

        int p = 0;
        for (int j = root; j;)
            if (T[j].key < T[i].key) {
                p = j;
                j = T[j].r;
            } else j = T[j].l;
        return p;
    }

    int successor(const int &i) const {
        if (!i) return 0;

        int s = 0;
        for (int j = root; j;)
            if (T[j].key > T[i].key) {
                s = j;
                j = T[j].l;
            } else j = T[j].r;
        return s;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    vector<Treap> instances(1e3 + 1);
    vector<int> cursor(1e3 + 1, 0);
    while (q--) {
        int id;
        char op;
        cin >> id >> op;

        if (op == 'a') {
            int from;
            cin >> from;

            instances[id] = instances[from];
            cursor[id] = 0;
        } else if (op == '?') {
            int key;
            cin >> key;

            cursor[id] = instances[id].find(key);
        } else if (op == 'l') {
            int key;
            cin >> key;

            cursor[id] = instances[id].lower_bound(key);
        } else if (op == 'u') {
            int key;
            cin >> key;

            cursor[id] = instances[id].upper_bound(key);
        } else if (op == 'i') {
            int key, val;
            cin >> key >> val;

            cursor[id] = instances[id].insert(key, val);
        } else if (op == 'e') {
            if (cursor[id]) {
                instances[id].erase(cursor[id]);
                cursor[id] = 0;
            }
        } else if (op == 'f') cursor[id] = instances[id].front();
        else if (op == 'b') cursor[id] = instances[id].back();
        else if (op == '>') cursor[id] = instances[id].successor(cursor[id]);
        else if (op == '<') cursor[id] = instances[id].predecessor(cursor[id]);
        else if (op == 'r') {
            if (cursor[id]) cout << instances[id].rank(cursor[id]) << "\n";
            else cout << "-\n";
        } else if (op == 'k') {
            int k;
            cin >> k;

            cursor[id] = instances[id].find_by_order(k);
        } else if (op == 'g') {
            if (cursor[id]) cout << instances[id].get(cursor[id]) << "\n";
            else cout << "-\n";
        } else if (op == 's') {
            int val;
            cin >> val;

            if (cursor[id]) instances[id].set(cursor[id], val);
        } else if (op == 'z') cout << instances[id].size() << "\n";
    }
}
