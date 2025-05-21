#include <bits/stdc++.h>
using namespace std;

struct SizeBalancedTree {
    struct SBTNode {
        int l, r, size;
        pair<long long, int> key;

        long long subtree_sum;
        vector<long long> sums;

        SBTNode(int K) : l(0), r(0), size(1), key{0, 0}, subtree_sum(0), sums(K, 0) {}

        auto & operator=(const pair<long long, int> &k) {
            l = r = 0;
            size = 1;
            key = k;
            fill(sums.begin(), sums.end(), 0);
            subtree_sum = sums[0] = -k.first;
            return *this;
        }
    };

    int root, nodes, K;
    vector<SBTNode> SBT;
    stack<int> recycled;

    SizeBalancedTree(int n, int k) : root(0), nodes(1), K(1 << k), SBT(n + 1, 1 << k) {}

    int size(int i) const {
        return !i ? 0 : SBT[i].size;
    }

    long long subtree_sum(int i) const {
        return !i ? 0 : SBT[i].subtree_sum;
    }

    int node(const pair<int, int> &key) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else i = nodes++;

        SBT[i] = key;
        return i;
    }

    void pull(int i) {
        int l = SBT[i].l, r = SBT[i].r;

        SBT[i].size = size(l) + size(r) + 1;
        SBT[i].subtree_sum = subtree_sum(l) + subtree_sum(r) - SBT[i].key.first;

        for (int k = 0, a = size(l) % K, b = (size(l) + 1) % K; k < K; k++) {
            auto s = SBT[l].sums[k];
            if (k == a) s -= SBT[i].key.first;
            SBT[i].sums[k] = s += SBT[r].sums[(k - b + K) % K];
        }
    }

    int rotate_left(int i) {
        int j = SBT[i].r;
        SBT[i].r = SBT[j].l;
        SBT[j].l = i;
        pull(i);
        pull(j);
        return j;
    }

    int rotate_right(int i) {
        int j = SBT[i].l;
        SBT[i].l = SBT[j].r;
        SBT[j].r = i;
        pull(i);
        pull(j);
        return j;
    }

    int balance_left(int i) {
        if (!i) return 0;
        int &l = SBT[i].l, &r = SBT[i].r;
        if (!l) return i;

        int ll = SBT[l].l, lr = SBT[l].r;
        if (ll && size(ll) > size(r)) i = rotate_right(i);
        else if (lr && size(lr) > size(r)) {
            l = rotate_left(l);
            i = rotate_right(i);
        } else return i;

        l = balance_left(l);
        r = balance_right(r);
        i = balance_left(i);
        i = balance_right(i);
        return i;
    }

    int balance_right(int i) {
        if (!i) return 0;
        int &l = SBT[i].l, &r = SBT[i].r;
        if (!r) return i;

        int rr = SBT[r].r, rl = SBT[r].l;
        if (rr && size(rr) > size(l)) i = rotate_left(i);
        else if (rl && size(rl) > size(l)) {
            r = rotate_right(r);
            i = rotate_left(i);
        } else return i;

        l = balance_left(l);
        r = balance_right(r);
        i = balance_left(i);
        i = balance_right(i);
        return i;
    }

    int insert(const pair<long long, int> &key) {
        return root = insert(root, key);
    }

    int insert(int i, const pair<long long, int> &key) {
        if (!i) return node(key);
        if (key < SBT[i].key) {
            SBT[i].l = insert(SBT[i].l, key);
            pull(i);
            return balance_left(i);
        } else {
            SBT[i].r = insert(SBT[i].r, key);
            pull(i);
            return balance_right(i);
        }
    }

    int erase(const pair<long long, int> &key) {
        return root = erase(root, key);
    }

    int erase(int i, const pair<long long, int> &key) {
        if (!i) return 0;
        if (SBT[i].key == key) {
            if (!SBT[i].l || !SBT[i].r) {
                recycled.emplace(i);
                return SBT[i].l ^ SBT[i].r;
            } else {
                int c = SBT[i].r;
                while (SBT[c].l) c = SBT[c].l;
                SBT[i].key = SBT[c].key;
                SBT[i].r = erase(SBT[i].r, SBT[c].key);
            }
        } else if (key < SBT[i].key) SBT[i].l = erase(SBT[i].l, key);
        else SBT[i].r = erase(SBT[i].r, key);

        pull(i);
        return key < SBT[i].key ? balance_left(i) : balance_right(i);
    }

    auto & operator[](int i) {
        return SBT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q, k;
    cin >> Q >> k;

    int root = 0;
    SizeBalancedTree sbt(Q, k);
    vector<long long> skill(1e6 + 1);
    while (Q--) {
        int q, i;
        cin >> q >> i;

        if (q == 1) {
            int s;
            cin >> s;

            skill[i] = s;
            root = sbt.insert({-s, i});
        } else root = sbt.erase({-skill[i], i});

        cout << sbt[root].subtree_sum - sbt[root].sums[0] << "\n" << flush;
    }
}
