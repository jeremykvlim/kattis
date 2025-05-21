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

struct WeightBalancedTree {
    struct WBTNode {
        int l, r, size, virtual_size, a, b, rem;
        long long sum;

        WBTNode(int l = 0, int r = 0) : l(l), r(r), size(1), virtual_size(-1), a(-1), b(-1), rem(-1), sum(-1) {}

        auto & operator=(const array<int, 4> &arr) {
            l = r = 0;
            size = 1;
            virtual_size = arr[0];
            a = arr[1];
            b = arr[2];
            rem = arr[3];
            int c = (a + rem) % b;
            sum = linear_mod_sum(a, arr[0], b) + (long long) arr[0] * c - (floor_division_sum(arr[0], a, c, b) - floor_division_sum(arr[0], a, 0, b)) * b;
            return *this;
        }
    };

    int root, nodes;
    vector<WBTNode> WBT;
    stack<int> recycled;

    WeightBalancedTree(int n, int m) : root(0), nodes(1), WBT(n + 1) {
        root = node(m, 0, 1);
    }

    int size(int i) const {
        return !i ? 0 : WBT[i].size;
    }

    int virtual_size(int i) const {
        return !i ? 0 : WBT[i].virtual_size;
    }

    long long sum(int i) const {
        return !i ? 0 : WBT[i].sum;
    }

    void pull(int i) {
        int l = WBT[i].l, r = WBT[i].r;

        WBT[i].size = size(l) + size(r) + 1;
        WBT[i].virtual_size = virtual_size(l) + virtual_size(r);
        WBT[i].sum = sum(l) + sum(r);
    }

    int node(int l, int r) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else i = nodes++;

        WBT[i] = WBTNode(l, r);
        return i;
    }

    int node(int virtual_size, int a, int b, int rem = 0) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else i = nodes++;

        WBT[i] = {virtual_size, a, b, rem};
        return i;
    }

    int splice(int l, int r) {
        int i = node(l, r);
        pull(i);
        return i;
    }

    pair<int, int> excise(int i) {
        recycled.emplace(i);
        return pair(WBT[i].l, WBT[i].r);
    }

    bool heavy(int s1, int s2) {
        return s1 > s2 * 3;
    }

    int meld(int i, int j) {
        if (!i || !j) return i ^ j;

        if (heavy(size(i), size(j))) {
            auto [l, r] = excise(i);
            if (heavy(size(r) + size(j), size(l))) {
                auto [rl, rr] = excise(r);
                return meld(meld(l, rl), meld(rr, j));
            }
            return meld(l, meld(r, j));
        }

        if (heavy(size(j), size(i))) {
            auto [l, r] = excise(j);
            if (heavy(size(l) + size(i), size(r))) {
                auto [ll, lr] = excise(l);
                return meld(meld(i, ll), meld(lr, r));
            }
            return meld(meld(i, l), r);
        }

        return splice(i, j);
    }

    pair<int, int> virtual_split(int i, int k) {
        if (!i) return {0, 0};
        if (!k) return {0, i};
        if (k == virtual_size(i)) return {i, 0};

        if (size(i) == 1) {
            int j = node(WBT[i].virtual_size - k, WBT[i].a, WBT[i].b, ((long long) WBT[i].a * k + WBT[i].rem) % WBT[i].b);
            WBT[i].virtual_size = k;
            WBT[i].sum -= WBT[j].sum;
            return {i, j};
        }

        auto [a, b] = excise(i);
        if (k <= virtual_size(a)) {
            auto [ll, rr] = virtual_split(a, k);
            return {ll, meld(rr, b)};
        } else {
            auto [ll, rr] = virtual_split(b, k - virtual_size(a));
            return {meld(a, ll), rr};
        }
    }

    void destroy(int i) {
        if (!i) return;
        destroy(WBT[i].l);
        destroy(WBT[i].r);
        recycled.emplace(i);
    }

    void modify(int l, int r, int a, int b) {
        auto [rl, rr] = virtual_split(root, r);
        auto [ll, lr] = virtual_split(rl, l - 1);
        destroy(lr);
        root = meld(meld(ll, node(r - l + 1, a, b)), rr);
    }

    long long range_query(int l, int r) {
        auto [rl, rr] = virtual_split(root, r);
        auto [ll, lr] = virtual_split(rl, l - 1);
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

    WeightBalancedTree wbt(4 * Q + 1, n);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int l, r, a, b;
            cin >> l >> r >> a >> b;

            wbt.modify(l, r, a, b);
        } else if (q == 2) {
            int l, r;
            cin >> l >> r;
            cout << wbt.range_query(l, r) << "\n";
        }
    }
}
