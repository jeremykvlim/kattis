#include <bits/stdc++.h>
using namespace std;

long long hilbert_index(int x, int y, int z) {
    int lg = __lg(max({x, y, z}));

    array<int, 3> coords{x, y, z};
    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        for (int i = 2; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                int t = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= t;
                coords[i] ^= t;
            }

    for (int i = 1; i < 3; i++) coords[i] ^= coords[i - 1];
    int m = 0;
    for (int mask = 1 << lg; mask > 1; mask >>= 1)
        if (coords[2] & mask) m ^= mask - 1;
    for (int i = 0; i < 3; i++) coords[i] ^= m;

    auto h = 0LL;
    for (int b = lg; ~b; b--)
        for (int i = 0; i < 3; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

struct QueryDecomposition {
    int size;
    vector<array<int, 4>> queries;

    QueryDecomposition(int n, const vector<array<int, 4>> &queries) : size(cbrt(n) * cbrt(n)), queries(queries) {}

    vector<int> mo(vector<int> a, const vector<array<int, 3>> &updates, int k) {
        int Q = queries.size();
        vector<int> answers(Q), freq(a.size() + 1, 0);
        vector<long long> indices(Q);
        for (int q = 0; q < Q; q++) {
            auto [l, r, t, i] = queries[q];
            indices[q] = hilbert_index(l / size, r / size, t / size);
        }
        vector<int> order(Q);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return indices[i] < indices[j]; });

        int L = 0, R = -1, T = 0, ans = 0;

        auto update = [&](int i) {
            auto [A, B, prev] = updates[i];
            if (L <= A && A <= R) {
                int p = a[A];
                if (freq[p] == 1) ans--;
                freq[p]--;
                if (freq[p] == k) ans++;
                p = B;
                if (!freq[p]) ans++;
                freq[p]++;
                if (freq[p] == k + 1) ans--;
            }
            a[A] = B;
        };

        auto revert = [&](int i) {
            auto [A, B, prev] = updates[i];
            if (L <= A && A <= R) {
                int p = a[A];
                if (freq[p] == 1) ans--;
                freq[p]--;
                if (freq[p] == k) ans++;
                p = prev;
                if (!freq[p]) ans++;
                freq[p]++;
                if (freq[p] == k + 1) ans--;
            }
            a[A] = prev;
        };

        auto add = [&](int i) {
            int p = a[i];
            if (!freq[p]) ans++;
            freq[p]++;
            if (freq[p] == k + 1) ans--;
        };

        auto remove = [&](int i) {
            int p = a[i];
            if (freq[p] == k + 1) ans++;
            freq[p]--;
            if (!freq[p]) ans--;
        };

        for (int q : order) {
            auto [l, r, t, i] = queries[q];
            while (T < t) update(T++);
            while (T > t) revert(--T);
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

    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;
        pi--;
    }

    auto temp = p;
    vector<array<int, 4>> queries;
    vector<array<int, 3>> updates;
    while (m--) {
        int q, a, b;
        cin >> q >> a >> b;
        a--;
        b--;

        if (q == 1) {
            updates.push_back({a, b, temp[a]});
            temp[a] = b;
        } else queries.push_back({a, b, (int) updates.size(), (int) queries.size()});
    }

    QueryDecomposition qd(n, queries);
    for (int count : qd.mo(p, updates, k)) cout << count << "\n";
}
