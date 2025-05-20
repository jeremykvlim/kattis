#include <bits/stdc++.h>
using namespace std;

long long floor_division_sum(long long n, int a, int c, int d) {
    auto sum = 0LL;
    while (n) {
        if (a >= d) {
            sum += (a / d) * (__int128) (n * (n - 1)) / 2;
            a %= d;
        }
        if (c >= d) {
            sum += n * (c / d);
            c %= d;
        }

        auto y_max = a * n + c;
        if (y_max < d) break;
        n = y_max / d;
        c = y_max % d;
        swap(a, d);
    }
    return sum;
}

__int128 linear_mod_sum(int a, int n, int mod) {
    return (__int128) a * n * (n - 1) / 2 - (__int128) mod * floor_division_sum(n, a, 0, mod);
}

struct SizeBalancedTree {
    struct SBTNode {
        int l, r, size;

        int a, b, rem, base_count, aggregate_count;
        long long base_sum, aggregate_sum;

        SBTNode() : l(0), r(0), size(1), a(-1), b(-1), rem(-1), base_count(-1), aggregate_count(-1), base_sum(-1), aggregate_sum(-1) {}

        auto & operator=(const array<int, 4> &arr) {
            l = r = 0;
            size = 1;
            aggregate_count = base_count = arr[0];
            a = arr[1];
            b = arr[2];
            rem = arr[3];
            int c = (a + rem) % b;
            aggregate_sum = base_sum = linear_mod_sum(a, arr[0], b) + (long long) arr[0] * c - (floor_division_sum(arr[0], a, c, b) - floor_division_sum(arr[0], a, 0, b)) * b;
            return *this;
        }
    };

    int root, nodes;
    vector<SBTNode> SBT;
    vector<int> recycled;

    SizeBalancedTree(int n, int m) : root(0), nodes(1), SBT(n + 1) {
        root = node(m);
    }

    int node(int count, int a = 0, int b = 1, int rem = 0) {
        int i;
        if (!recycled.empty()) {
            i = recycled.back();
            recycled.pop_back();
        } else i = nodes++;

        SBT[i] = {count, a, b, rem};
        return i;
    }

    int size(int i) const {
        return !i ? 0 : SBT[i].size;
    }

    int count(int i) const {
        return !i ? 0 : SBT[i].aggregate_count;
    }

    long long sum(int i) const {
        return !i ? 0 : SBT[i].aggregate_sum;
    }

    void pull(int i) {
        int l = SBT[i].l, r = SBT[i].r;

        SBT[i].size = size(l) + size(r) + 1;
        SBT[i].aggregate_count = count(l) + count(r) + SBT[i].base_count;
        SBT[i].aggregate_sum = sum(l) + sum(r) + SBT[i].base_sum;
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

    void destroy(int i) {
        if (!i) return;

        destroy(SBT[i].l);
        destroy(SBT[i].r);
        recycled.emplace_back(i);
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;

        if (size(i) > size(j)) {
            SBT[i].r = meld(SBT[i].r, j);
            pull(i);
            return balance_left(i);
        } else {
            SBT[j].l = meld(i, SBT[j].l);
            pull(j);
            return balance_right(j);
        }
    }

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};

        int diff = k - count(SBT[i].l);
        if (diff < 0) {
            auto [ll, lr] = split(SBT[i].l, k);
            SBT[i].l = lr;
            pull(i);
            return {ll, i};
        } else if (diff > SBT[i].base_count) {
            auto [rl, rr] = split(SBT[i].r, diff - SBT[i].base_count);
            SBT[i].r = rl;
            pull(i);
            return {i, rr};
        } else if (diff == SBT[i].base_count) {
            int r = SBT[i].r;
            SBT[i].r = 0;
            pull(i);
            return {i, r};
        } else {
            int ml = node(diff, SBT[i].a, SBT[i].b, SBT[i].rem),
                mr = node(SBT[i].base_count - diff, SBT[i].a, SBT[i].b, ((long long) SBT[i].a * diff + SBT[i].rem) % SBT[i].b);

            SBT[ml].l = SBT[i].l;
            SBT[mr].r = SBT[i].r;
            pull(ml);
            pull(mr);
            recycled.emplace_back(i);
            return {ml, mr};
        }
    }

    void modify(int l, int r, int a, int b) {
        auto [rl, rr] = split(root, r);
        auto [ll, lr] = split(rl, l - 1);
        destroy(lr);
        root = meld(meld(ll, node(r - l + 1, a, b)), rr);
    }

    long long range_query(int l, int r) {
        auto [rl, rr] = split(root, r);
        auto [ll, lr] = split(rl, l - 1);
        auto s = sum(lr);
        root = meld(meld(ll, lr), rr);
        return s;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    SizeBalancedTree sbt(3 * Q, n);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int l, r, a, b;
            cin >> l >> r >> a >> b;

            sbt.modify(l, r, a, b);
        } else {
            int l, r;
            cin >> l >> r;
            cout << sbt.range_query(l, r) << "\n";
        }
    }
}
