#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
    SparseTable(const vector<T> &v, function<T(T, T)> func) : f(func) {
        if (v.empty()) return;
        ST.resize(__lg(v.size()) + 1);
        ST[0] = v;
        for (int i = 1; i < ST.size(); i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j < ST[i].size(); j++) ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l);
        return f(ST[i][l], ST[i][r - (1 << i)]);
    }
};

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end()) + 1) + 1), order(n) {
        mid.resize(lg);
        pref.assign(lg, vector<int>(n + 1, 0));

        vector<pair<T, int>> temp(n);
        for (int i = 0; i < n; i++) order[i] = {a[i], i};
        for (int b = lg - 1; ~b; b--) {
            for (int i = 0; i < n; i++) pref[b][i + 1] = pref[b][i] + ((order[i].first >> b) & 1);

            int zeroes = 0, ones = mid[b] = n - pref[b][n];
            for (int i = 0; i < n; i++) temp[((order[i].first >> b) & 1) ? ones++ : zeroes++] = order[i];
            order.swap(temp);
        }
    }

    tuple<T, int, int, int> quantile(int l, int r, int k) {
        T v = 0;
        for (int b = lg - 1; ~b; b--) {
            int zeroes = (r - pref[b][r]) - (l - pref[b][l]);
            if (k >= zeroes) {
                k -= zeroes;
                v |= ((T) 1) << b;
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return {v, k, r - l, order[l + k].second};
    }

    int freq_less(int l, int r, T x) {
        int f = 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                f += (r - pref[b][r]) - (l - pref[b][l]);
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return f;
    }

    int freq(int l, int r, T x) {
        if ((((T) 1) << lg) <= x) return 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return r - l;
    }

    T lower_bound(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }

    T predecessor(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return !f ? -1 : get<0>(quantile(l, r, f - 1));
    }

    T successor(int l, int r, T x) {
        int f = freq_less(l, r, x) + freq(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        string s;
        cin >> n >> s;

        vector<long long> t(n);
        for (auto &ti : t) cin >> ti;

        vector<int> pref(n + 1, 0);
        vector<long long> diff(n, LLONG_MAX), gaps;
        for (int i = 0, j = -1; i < n; i++) {
            pref[i + 1] = pref[i] + (s[i] == 'W');

            if (i + 1 < n && s[i] == 'W' && s[i + 1] == 'M') diff[i] = t[i + 1] - t[i];
            if (s[i] == 'W') {
                gaps.emplace_back(!~j ? 0 : t[i] - t[j]);
                j = i;
            }
        }

        auto deduped = gaps;
        sort(deduped.begin(), deduped.end());
        deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

        vector<int> rank(gaps.size() + 1);
        for (int i = 0; i < gaps.size(); i++) rank[i] = lower_bound(deduped.begin(), deduped.end(), gaps[i]) - deduped.begin();
        rank.back() = deduped.size();

        WaveletMatrix<int> wm(rank.size(), rank);
        SparseTable<long long> st(diff, [](auto x, auto y) { return min(x, y); });

        int q;
        cin >> q;

        while (q--) {
            int l, r;
            cin >> l >> r;
            l--;
            r--;

            long long d = l == r ? LLONG_MAX : st.range_query(l, r);
            if (d == LLONG_MAX) cout << (pref[l] < pref[r + 1]) << " -1\n";
            else cout << pref[r + 1] - pref[l] - wm.freq_less(pref[l] + 1, pref[r + 1], lower_bound(deduped.begin(), deduped.end(), d) - deduped.begin()) << " " << d << "\n";
        }
    }
}
