#include <bits/stdc++.h>
using namespace std;

template <typename... I>
auto hilbert_index(I... c) {
    using T = common_type_t<I...>;
    constexpr int D = sizeof...(I);
    array<T, D> coords{(T) c...};
    T c_max = max({c...});
    int b = 0;
    if (c_max)
        while (c_max >>= 1) b++;

    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                T m = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= m;
                coords[i] ^= m;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    T m = 0;
    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    T h = 0;
    for (; ~b; b--)
        for (int i = 0; i < D; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

struct QueryDecomposition {
    int size;
    vector<array<int, 3>> queries;

    QueryDecomposition(int n, const vector<array<int, 3>> &queries) : size(ceil(sqrt(n))), queries(queries) {}

    vector<int> mo(const vector<int> &a, const vector<int> &b, int k) {
        int Q = queries.size();
        vector<int> answers(Q), hilbert_order(Q);
        for (int q = 0; q < Q; q++) {
            auto [l, r, i] = queries[q];
            hilbert_order[q] = hilbert_index(l / size, r / size);
        }
        vector<int> order(Q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return hilbert_order[i] < hilbert_order[j]; });

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
