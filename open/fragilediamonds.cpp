#include <bits/stdc++.h>
using namespace std;

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        int l, r;
        unsigned long long prio;
        pair<long long, int> key;
        long long x_plus, x_minus, x_max_plus, x_max_minus;

        TreapNode() : l(0), r(0), prio(0), key{0, 0}, x_plus(0), x_minus(0), x_max_plus(LLONG_MIN), x_max_minus(LLONG_MIN) {}

        auto & operator=(const pair<pair<long long, int>, long long> &k) {
            prio = rng();
            key = k.first;
            x_plus = x_max_plus = k.first.first + 2 * k.second;
            x_minus = x_max_minus = -k.first.first + 2 * k.second;
            return *this;
        }
    };

    int root, nodes;
    vector<TreapNode> T;

    Treap(int n) : root(0), nodes(1), T(n + 1) {}

    long long max_plus(int i) const {
        return !i ? LLONG_MIN : T[i].x_max_plus;
    }

    long long max_minus(int i) const {
        return !i ? LLONG_MIN : T[i].x_max_minus;
    }

    int pull(int i) {
        if (!i) return 0;
        int l = T[i].l, r = T[i].r;
        T[i].x_max_plus = max({T[i].x_plus, max_plus(l), max_plus(r)});
        T[i].x_max_minus = max({T[i].x_minus, max_minus(l), max_minus(r)});
        return i;
    }

    pair<int, int> split(int i, const pair<long long, int> &key) {
        if (!i) return {0, 0};

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
            T[i].r = meld(T[i].r, j);
            return pull(i);
        } else {
            T[j].l = meld(i, T[j].l);
            return pull(j);
        }
    }

    int insert(const pair<long long, int> &key, long long y) {
        int i = nodes++;
        T[i] = {key, y};

        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        return i;
    }

    int erase(int i, const pair<long long, int> &key) {
        if (!i) return 0;
        if (T[i].key == key) return meld(T[i].l, T[i].r);

        if (T[i].key > key) T[i].l = erase(T[i].l, key);
        else T[i].r = erase(T[i].r, key);

        return pull(i);
    }

    pair<int, int> erase_by_x_plus(int i, long long x) {
        if (!i || max_plus(i) <= x) return {i, 0};

        int l = T[i].l, r = T[i].r;
        if (l && max_plus(l) > x) {
            auto [ll, j] = erase_by_x_plus(l, x);
            T[i].l = ll;
            return {pull(i), j};
        } else if (T[i].x_plus <= x) {
            auto [rr, j] = erase_by_x_plus(r, x);
            T[i].r = rr;
            return {pull(i), j};
        }
        return {erase(i, T[i].key), i};
    }

    pair<int, int> erase_by_x_minus(int i, long long x) {
        if (!i || max_minus(i) <= x) return {i, 0};

        int l = T[i].l, r = T[i].r;
        if (l && max_minus(l) > x) {
            auto [ll, j] = erase_by_x_minus(l, x);
            T[i].l = ll;
            return {pull(i), j};
        } else if (T[i].x_minus <= x) {
            auto [rr, j] = erase_by_x_minus(r, x);
            T[i].r = rr;
            return {pull(i), j};
        }
        return {erase(i, T[i].key), i};
    }

    vector<int> query(long long x, long long y, int i) {
        vector<int> v;
        auto [l, r] = split(root, {x - 2 * y, INT_MAX});
        auto [rl, rr] = split(r, {x + 2 * y - 1, INT_MAX});
        auto [rll, rlr] = split(rl, {x, INT_MAX});

        while (rll && max_plus(rll) > x) {
            auto [ll, j] = erase_by_x_plus(rll, x);
            rll = ll;
            v.emplace_back(T[j].key.second);
        }
        while (rlr && max_minus(rlr) > -x) {
            auto [lr, j] = erase_by_x_minus(rlr, -x);
            rlr = lr;
            v.emplace_back(T[j].key.second);
        }

        root = meld(l, meld(rll, meld(rlr, rr)));
        insert({x, i}, y);
        return v;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Treap treap(n);
    for (int i = 1; i <= n; i++) {
        long long x, y;
        cin >> x >> y;

        auto broken = treap.query(x, y, i);
        sort(broken.begin(), broken.end());
        cout << broken.size() << " ";
        for (int j : broken) cout << j << " ";
        cout << "\n";
    }
}
