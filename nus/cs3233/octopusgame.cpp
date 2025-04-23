#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static mt19937_64 rng;

    struct TreapNode {
        int l, r, size;
        unsigned long long prio;
        pair<long long, int> key;

        long long subtree_sum;
        vector<long long> sums;

        TreapNode(int K) : l(0), r(0), size(1), prio(-1), key{-1, -1}, subtree_sum(0), sums(K, 0) {}

        auto & operator=(const pair<long long, int> &k) {
            prio = rng();
            key = k;
            subtree_sum = sums[0] = k.first;
            return *this;
        }
    };

    int K, root, nodes;
    vector<TreapNode> T;

    Treap(int n, int k) : root(0), nodes(1), K(1 << k), T(n + 1, TreapNode(1 << k)) {}

    int size(int i) {
        return !i ? 0 : T[i].size;
    }

    long long subtree_sum(int i) {
        return !i ? 0 : T[i].subtree_sum;
    }

    int pull(int i) {
        if (!i) return 0;

        T[i].size = size(T[i].l) + size(T[i].r) + 1;
        T[i].subtree_sum = subtree_sum(T[i].l) + subtree_sum(T[i].r) + T[i].key.first;

        for (int k = 0, a = size(T[i].l) % K, b = (size(T[i].l) + 1) % K; k < K; k++) {
            auto s = T[T[i].l].sums[k];
            if (k == a) s += T[i].key.first;
            T[i].sums[k] = s += T[T[i].r].sums[(k - b + K) % K];
        }
        return i;
    }

    void split(int i, pair<long long, int> key, int &l, int &r) {
        if (!i) l = r = 0;
        else if (T[i].key > key) {
            split(T[i].r, key, T[i].r, r);
            pull(l = i);
        } else {
            split(T[i].l, key, l, T[i].l);
            pull(r = i);
        }
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;

        if (T[i].prio > T[j].prio) {
            T[i].r = meld(T[i].r, j);
            return pull(i);
        } else {
            T[j].l = meld(i, T[j].l);
            return pull(j);
        }
    }

    int insert(const pair<long long, int> &key) {
        int i = nodes++;
        T[i] = key;

        int l = 0, r = 0;
        split(root, T[i].key, l, r);
        return root = meld(meld(l, i), r);
    }

    int erase(const pair<long long, int> &key) {
        return root = erase(root, key);
    }

    int erase(int i, const pair<long long, int> &key) {
        if (!i) return 0;
        if (T[i].key == key) return meld(T[i].l, T[i].r);

        if (T[i].key < key) T[i].l = erase(T[i].l, key);
        else T[i].r = erase(T[i].r, key);

        return pull(i);
    }

    auto & operator[](int i) {
        return T[i];
    }
};

mt19937_64 Treap::rng(random_device{}());

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q, k;
    cin >> Q >> k;

    int root = 0;
    Treap treap(Q, k);
    vector<long long> skill(1e6 + 1);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i, s;
            cin >> i >> s;

            root = treap.insert({skill[i] = s, i});
        } else {
            int i;
            cin >> i;

            root = treap.erase({skill[i], i});
            skill[i] = 0;
        }
        cout << treap[root].subtree_sum - treap[root].sums[0] << "\n" << flush;
    }
}
