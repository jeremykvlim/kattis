#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        int size;
        unsigned long long prio;
        int key, val;

        TreapNode(int k = 0, int v = 0) : family{0, 0, 0}, size(1), prio(rng()), key(k), val(v) {}
    };

    int root, nodes;
    vector<TreapNode> T;

    Treap() : root(0), nodes(1), T(1) {}
    Treap(int n) : root(0), nodes(1), T(n + 1) {}

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
        if (nodes >= T.size()) T.resize(nodes << 1);
        T[i] = {key, val};
        return i;
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

    int find(const int &key) const {
        for (int i = root; i; i = key < T[i].key ? T[i].family[0] : T[i].family[1])
            if (key == T[i].key) return i;
        return 0;
    }

    int insert(const int &key, const int &val) {
        int i = find(key);
        if (i) return i;

        i = node(key, val);
        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        T[root].family[2] = 0;
        return i;
    }

    int erase(const int &i) {
        if (!i) return 0;
        root = erase(root, T[i].key);
        T[root].family[2] = 0;
        return root;
    }

    int erase(int i, const int &key) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        if (T[i].key == key) {
            int m = meld(l, r);
            if (m) T[m].family[2] = 0;
            return m;
        }

        if (T[i].key > key) attach(i, 0, erase(l, key));
        else attach(i, 1, erase(r, key));
        return i;
    }

    int lower_bound(const int &key) const {
        int k = 0;
        for (int i = root; i;)
            if (T[i].key >= key) {
                k = i;
                i = T[i].family[0];
            } else i = T[i].family[1];
        return k;
    }

    int upper_bound(const int &key) const {
        int k = 0;
        for (int i = root; i;)
            if (T[i].key > key) {
                k = i;
                i = T[i].family[0];
            } else i = T[i].family[1];
        return k;
    }

    int front() const {
        if (!root) return 0;
        int i = root;
        for (; T[i].family[0]; i = T[i].family[0]);
        return i;
    }

    int back() const {
        if (!root) return 0;
        int i = root;
        for (; T[i].family[1]; i = T[i].family[1]);
        return i;
    }

    int rank(int i) const {
        if (!i) return 0;
        return order_of_key(T[i].key);
    }

    int order_of_key(const int &key) const {
        int rank = 0;
        for (int i = root; i;)
            if (key <= T[i].key) i = T[i].family[0];
            else {
                rank += size(T[i].family[0]) + 1;
                i = T[i].family[1];
            }
        return rank;
    }

    int find_by_order(int k) const {
        for (int i = root; i;) {
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

    int predecessor(int i) const {
        if (!i) return 0;
        if (T[i].family[0]) {
            for (i = T[i].family[0]; T[i].family[1]; i = T[i].family[1]);
            return i;
        }
        int p = T[i].family[2];
        for (; p && T[p].family[0] == i; i = p, p = T[p].family[2]);
        return p;
    }

    int successor(int i) const {
        if (!i) return 0;
        if (T[i].family[1]) {
            for (i = T[i].family[1]; T[i].family[0]; i = T[i].family[0]);
            return i;
        }
        int p = T[i].family[2];
        for (; p && T[p].family[1] == i; i = p, p = T[p].family[2]);
        return p;
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
