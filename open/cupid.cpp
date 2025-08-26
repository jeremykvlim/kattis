#include <bits/stdc++.h>
using namespace std;

template <typename... I>
long long hilbert_index(I... c) {
    constexpr int D = sizeof...(I);
    array<int, D> coords{c...};
    int lg = __lg(max({c...}));

    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                int t = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= t;
                coords[i] ^= t;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    int m = 0;
    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    auto h = 0LL;
    for (int b = lg; ~b; b--)
        for (int i = 0; i < D; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

struct QueryDecomposition {
    int size;
    vector<array<int, 3>> queries;

    QueryDecomposition(int n, const vector<array<int, 3>> &queries) : size(ceil(sqrt(n))), queries(queries) {}

    vector<int> mo(const vector<int> &a, const vector<int> &b, int k) {
        int Q = queries.size();
        vector<int> answers(Q);
        vector<long long> indices(Q);
        for (int q = 0; q < Q; q++) {
            auto [l, r, i] = queries[q];
            indices[q] = hilbert_index(l / size, r / size);
        }
        vector<int> order(Q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return indices[i] < indices[j]; });

        int L = 0, R = -1, ans = 0;

        vector<int> c(k, 0), d(k, 0);
        auto add = [&](int i) {
            ans -= min(c[a[i]], d[a[i]]);
            c[a[i]]++;
            ans += min(c[a[i]], d[a[i]]);
            ans -= min(c[b[i]], d[b[i]]);
            d[b[i]]++;
            ans += min(c[b[i]], d[b[i]]);
        };

        auto remove = [&](int i) {
            ans -= min(c[a[i]], d[a[i]]);
            c[a[i]]--;
            ans += min(c[a[i]], d[a[i]]);
            ans -= min(c[b[i]], d[b[i]]);
            d[b[i]]--;
            ans += min(c[b[i]], d[b[i]]);
        };

        for (int q : order) {
            auto [l, r, i] = queries[q];
            while (L > l) add(--L);
            while (R < r) add(++R);
            while (L < l) remove(L++);
            while (R > r) remove(R--);
            answers[i] = ans;
        }

        return answers;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) cin >> bi;

    vector<array<int, 3>> queries(m);
    for (int j = 0; j < m; j++) {
        auto &[l, r, i] = queries[j];
        cin >> l >> r;

        i = j;
    }

    QueryDecomposition qd(n, queries);
    for (int couples : qd.mo(a, b, k)) cout << couples << "\n";
}
