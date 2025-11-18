#include <bits/stdc++.h>
using namespace std;

template <typename C>
struct CartesianTree {
    int root;
    vector<array<int, 3>> CT;

    template <typename T>
    CartesianTree(const vector<T> &a) {
        int n = a.size();
        CT.resize(n, {0, 0, 0});

        for (int i = 1; i < n; i++) {
            auto &[l, r, p] = CT[i];
            p = i - 1;
            while (C()(a[p], a[i])) p = CT[p][2];
            l = CT[p][1];
            CT[p][1] = CT[l][2] = i;
        }
        root = CT[0][1];
    }

    auto & operator[](int i) {
        return CT[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    cin >> n >> Q;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<int> b(n, 1e9);
    for (int i = 1; i < n; i++) b[i] = max(a[i], a[i + 1]);

    vector<int> results(Q), sum(Q, 0);
    vector<vector<pair<int, int>>> sweep_l(n), sweep_r(n);
    for (int q = 0; q < Q; q++) {
        int l, r;
        cin >> l >> r;

        if (l == r) results[q] = a[l];
        else {
            sweep_l[l].emplace_back(r - 1, q);
            sweep_r[r - 1].emplace_back(l, q);
        }
    }

    auto div_p2 = [&](int v, int exponent) {
        return v >> min(31, exponent);
    };

    CartesianTree<less<>> ct1(b);
    vector<int> dp_up(n);
    auto calc_l = [&](auto &ct, int i) {
        auto [l, r, p] = ct[i];
        if (!l) return 0;
        return div_p2(dp_up[l], b[i] - b[l]);
    };

    auto calc_r = [&](auto &ct, int i) {
        auto [l, r, p] = ct[i];
        if (!r) return 0;
        return div_p2(dp_up[r], b[i] - b[r]);
    };

    auto dp1 = [&](auto &&self, auto &ct, int i) -> void {
        if (!i) return;
        auto [l, r, p] = ct[i];
        self(self, ct, l);
        self(self, ct, r);
        dp_up[i] = calc_l(ct, i) + calc_r(ct, i) + 1;
    };
    dp1(dp1, ct1, ct1.root);

    vector<int> dp_down(n);
    auto dp2 = [&](auto &&self, auto &ct, int i) -> void {
        if (!i) return;
        auto [l, r, p] = ct[i];
        dp_down[i] = dp_down[p] + calc_l(ct, i) + 1;
        self(self, ct, l);
        self(self, ct, r);
    };
    dp2(dp2, ct1, ct1.root);

    deque<pair<int, int>> mono_dec;
    for (int i = n - 1; i; i--) {
        while (!mono_dec.empty() && b[mono_dec.back().first] <= b[i]) mono_dec.pop_back();
        mono_dec.emplace_back(i, calc_r(ct1, i) + 1);

        for (int k = mono_dec.size() - 1; k;) {
            int v = div_p2(mono_dec[k].second, b[mono_dec[k - 1].first] - b[mono_dec[k].first]) + calc_r(ct1, mono_dec[k - 1].first) + 1;
            if (mono_dec[k - 1].second != v) {
                mono_dec[k - 1].second = v;
                k--;
            } else break;
        }

        for (auto [r, q] : sweep_l[i]) {
            int k = lower_bound(mono_dec.begin(), mono_dec.end(), make_pair(r, INT_MAX), greater()) - mono_dec.begin();
            sum[q] -= dp_down[mono_dec[k].first];
            results[q] = b[mono_dec[k].first];
            if (k + 1 < mono_dec.size()) sum[q] += div_p2(mono_dec[k + 1].second, b[mono_dec[k].first] - b[mono_dec[k + 1].first]);
        }
    }

    CartesianTree<less_equal<>> ct2(b);
    fill(dp_up.begin(), dp_up.end(), 0);
    dp1(dp1, ct2, ct2.root);

    mono_dec.clear();
    for (int i = 1; i < n; i++) {
        while (!mono_dec.empty() && b[mono_dec.back().first] <= b[i]) mono_dec.pop_back();
        mono_dec.emplace_back(i, calc_l(ct2, i) + 1);

        for (int k = mono_dec.size() - 1; k;) {
            int v = div_p2(mono_dec[k].second, b[mono_dec[k - 1].first] - b[mono_dec[k].first]) + calc_l(ct2, mono_dec[k - 1].first) + 1;
            if (mono_dec[k - 1].second != v) {
                mono_dec[k - 1].second = v;
                k--;
            } else break;
        }

        for (auto [l, q] : sweep_r[i]) {
            int k = lower_bound(mono_dec.begin(), mono_dec.end(), make_pair(l, -1)) - mono_dec.begin();
            sum[q] += dp_down[mono_dec[k].first] + 1;
            if (k + 1 < mono_dec.size()) sum[q] += div_p2(mono_dec[k + 1].second, b[mono_dec[k].first] - b[mono_dec[k + 1].first]);
        }
    }

    for (int q = 0; q < Q; q++) cout << results[q] + __lg(sum[q] << 1 | 1) << "\n";
}
