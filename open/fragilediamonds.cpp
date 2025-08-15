#include <bits/stdc++.h>
using namespace std;

struct SizeBalancedTree {
    static inline vector<long long> y;

    struct SBTNode {
        int l, r, size;
        pair<long long, int> key;

        long long xm, xp, xl, xr;

        SBTNode() : l(0), r(0), size(1), key{0, 0}, xm(LLONG_MIN), xp(LLONG_MIN), xl(LLONG_MAX), xr(LLONG_MIN) {}

        auto & operator=(const pair<long long, int> &k) {
            l = r = 0;
            size = 1;
            key = k;
            auto [xi, i] = k;
            xm = -xi + 2 * y[i];
            xp = xi + 2 * y[i];
            xl = xr = xi;
            return *this;
        }
    };

    int root, nodes;
    vector<SBTNode> SBT;
    stack<int> recycled;

    SizeBalancedTree(int n) : root(0), nodes(1), SBT(n + 1) {
        y.resize(n);
    }

    int size(int i) const {
        return !i ? 0 : SBT[i].size;
    }

    long long x_minus(int i) const {
        return !i ? LLONG_MIN : SBT[i].xm;
    }

    long long x_plus(int i) const {
        return !i ? LLONG_MIN : SBT[i].xp;
    }

    long long x_min(int i) const {
        return !i ? LLONG_MAX : SBT[i].xl;
    }

    long long x_max(int i) const {
        return !i ? LLONG_MIN : SBT[i].xr;
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

        auto [xj, j] = SBT[i].key;
        SBT[i].xm = max({-xj + 2 * y[j], x_minus(l), x_minus(r)});
        SBT[i].xp = max({xj + 2 * y[j], x_plus(l), x_plus(r)});
        SBT[i].xl = min(xj, min(x_min(l), x_min(r)));
        SBT[i].xr = max(xj, max(x_max(l), x_max(r)));
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

    pair<int, int> query(int i, long long xl, long long xr, long long x, int sgn) {
        if (!i || (sgn == 1 ? x_plus(i) : x_minus(i)) <= sgn * x || x_max(i) < xl || x_min(i) > xr) return {0, -1};

        auto [xa, a] = query(SBT[i].l, xl, xr, x, sgn);
        if (~a) return {xa, a};

        auto [xj, j] = SBT[i].key;
        if (xl <= xj && xj <= xr)
            if (sgn * xj + 2 * y[j] > sgn * x) return {xj, j};

        auto [xb, b] = query(SBT[i].r, xl, xr, x, sgn);
        if (~b) return {xb, b};
        return {0, -1};
    }

    pair<int, int> query(long long xl, long long xr, long long x, int sgn) {
        auto [xj, j] = query(root, xl, xr, x, sgn);
        if (~j) erase({xj, j});
        return {xj, j};
    }

    auto & operator[](int i) {
        return SBT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    SizeBalancedTree sbt(n + 1);
    for (int i = 1; i <= n; i++) {
        long long x, y;
        cin >> x >> y;

        sbt.y[i] = y;
        vector<int> broken;
        for (;;) {
            auto [xj, j] = sbt.query(x - 2 * y + 1, x, x, 1);
            if (!~j) break;
            broken.emplace_back(j);
        }
        for (;;) {
            auto [xj, j] = sbt.query(x + 1, x + 2 * y - 1, x, -1);
            if (!~j) break;
            broken.emplace_back(j);
        }

        sbt.insert({x, i});
        sort(broken.begin(), broken.end());
        cout << broken.size() << " ";
        for (int j : broken) cout << j << " ";
        cout << "\n";
    }
}
