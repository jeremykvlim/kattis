#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        int size;
        unsigned long long prio;
        pair<long long, int> key;
        long long subtree_sum;
        array<long long, 16> sums;

        TreapNode(const pair<long long, int> &k = {0, 0}) : family{0, 0, 0}, size(1), prio(rng()), key(k), subtree_sum(k.first), sums{} {
            sums[0] = k.first;
        }
    };

    int root, nodes, K;
    vector<TreapNode> T;

    Treap(int n, int k) : root(0), nodes(1), K(1 << k), T(n + 1) {}

    int size(int i) {
        return !i ? 0 : T[i].size;
    }

    long long subtree_sum(int i) {
        return !i ? 0 : T[i].subtree_sum;
    }

    int pull(int i) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        T[i].size = size(l) + size(r) + 1;
        T[i].subtree_sum = subtree_sum(l) + subtree_sum(r) + T[i].key.first;

        int rem = size(r) % K, shift = (2 * K - rem - 1) % K;
        for (int k = 0; k < K; k++) T[i].sums[k] = T[l].sums[(k + shift) % K] + T[r].sums[k] + (k == rem ? T[i].key.first : 0);
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, const pair<long long, int> &key) {
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

    int insert(const pair<long long, int> &key) {
        int i = nodes++;
        T[i] = key;

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
            return m;
        }

        if (T[i].key > key) attach(i, 0, erase(l, key));
        else attach(i, 1, erase(r, key));
        return i;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q, k;
    cin >> Q >> k;

    Treap treap(Q, k);
    vector<long long> skill(1e6 + 1);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int i, s;
            cin >> i >> s;

            treap.insert({skill[i] = s, i});
        } else {
            int i;
            cin >> i;

            treap.erase({skill[i], i});
            skill[i] = 0;
        }
        cout << treap[treap.root].subtree_sum - treap[treap.root].sums[0] << "\n" << flush;
    }
}
