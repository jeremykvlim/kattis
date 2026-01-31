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

struct ImplicitTreap {
    static inline mt19937_64 rng{random_device{}()};

    static long long progression_sum(int n, int a, int mod, int rem) {
        int c = (a + rem) % mod;
        return (__int128) a * n * (n - 1) / 2 + (long long) n * c - (__int128) mod * floor_division_sum(n, a, c, mod);
    }

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        int base, len, a, mod, rem;
        long long base_sum, sum;

        TreapNode(int base = 0, int a = 0, int mod = 1, int rem = 0, long long base_sum = 0) : family{0, 0, 0}, prio(rng()),
                                                                                               base(base), len(base), a(a), mod(mod), rem(rem), base_sum(base_sum), sum(base_sum) {}
    };

    int root;
    vector<TreapNode> T;
    stack<int> recycled;

    ImplicitTreap(int n) : root(0), T(1) {
        root = node(n, 0, 1, 0, progression_sum(n, 0, 1, 0));
    }

    int base(int i) const {
        return !i ? 0 : T[i].base;
    }

    int len(int i) const {
        return !i ? 0 : T[i].len;
    }

    long long base_sum(int i) const {
        return !i ? 0 : T[i].base_sum;
    }

    long long sum(int i) const {
        return !i ? 0 : T[i].sum;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = T[i].family;
        T[i].len = len(l) + len(r) + T[i].base;
        T[i].sum = sum(l) + sum(r) + T[i].base_sum;
    }

    int node(int base, int a, int mod, int rem, long long base_sum) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else {
            T.emplace_back();
            i = T.size() - 1;
        }
        T[i] = {base, a, mod, rem, base_sum};
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
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

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        int sl = len(l);
        if (k < sl) {
            auto [ll, lr] = split(l, k);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else if (k > sl + base(i)) {
            auto [rl, rr] = split(r, k - sl - base(i));
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        } else if (k == sl) {
            attach(i, 0, 0);
            if (l) T[l].family[2] = 0;
            T[i].family[2] = 0;
            return {l, i};
        } else if (k == sl + base(i)) {
            attach(i, 1, 0);
            if (r) T[r].family[2] = 0;
            T[i].family[2] = 0;
            return {i, r};
        }

        int a = T[i].a, mod = T[i].mod, rem = T[i].rem, base_l = k - sl, base_r = T[i].base - base_l;
        if (base_l <= base_r) {
            auto sum = progression_sum(base_l, a, mod, rem);
            T[i].base = base_r;
            T[i].rem = ((long long) a * base_l + rem) % mod;
            T[i].base_sum -= sum;
            T[i].family[0] = 0;
            pull(i);
            return {insert(l, sl, node(base_l, a, mod, rem, sum)), i};
        } else {
            auto sum = progression_sum(base_r, a, mod, ((long long) a * base_l + rem) % mod);
            T[i].base = base_l;
            T[i].base_sum -= sum;
            T[i].family[1] = 0;
            pull(i);
            return {i, insert(r, 0, node(base_r, a, mod, ((long long) a * base_l + rem) % mod, sum))};
        }
    }

    int insert(int i, int pos, int m) {
        auto [l, r] = split(i, pos);
        return meld(meld(l, m), r);
    }

    void erase(int l, int r) {
        auto [rl, rr] = split(root, r);
        auto [ll, lr] = split(rl, l - 1);
        destroy(lr);
        root = meld(ll, rr);
    }

    void destroy(int i) {
        if (!i) return;
        auto [l, r, p] = T[i].family;
        destroy(l);
        destroy(r);
        T[i] = {};
        recycled.emplace(i);
    }

    void range_update(int l, int r, int a, int mod) {
        erase(l, r);
        root = insert(root, l - 1, node(r - l + 1, a, mod, 0, progression_sum(r - l + 1, a, mod, 0)));
    }

    long long pref_sum(int k) {
        auto s = 0LL;
        for (int i = root; i && k;) {
            auto [l, r, p] = T[i].family;

            int sl = len(l);
            if (k <= sl) {
                i = l;
                continue;
            }
            k -= sl;
            s += sum(l);

            if (k <= base(i)) return s + progression_sum(k, T[i].a, T[i].mod, T[i].rem);
            k -= base(i);
            s += base_sum(i);
            i = r;
        }
        return s;
    }

    long long range_query(int l, int r) {
        return pref_sum(r) - pref_sum(l - 1);
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    ImplicitTreap treap(n);
    while (Q--) {
        int q;
        cin >> q;

        if (q == 1) {
            int l, r, a, b;
            cin >> l >> r >> a >> b;

            treap.range_update(l, r, a, b);
        } else {
            int l, r;
            cin >> l >> r;
            cout << treap.range_query(l, r) << "\n";
        }
    }
}
