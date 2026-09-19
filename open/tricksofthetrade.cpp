#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<vector<T>> sum;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end()) + 1) + 1), order(n) {
        mid.resize(lg);
        pref.assign(lg, vector<int>(n + 1, 0));
        sum.assign(lg, vector<T>(n + 1, 0));

        vector<pair<T, int>> temp(n);
        for (int i = 0; i < n; i++) order[i] = {a[i], i};

        for (int b = lg - 1; ~b; b--) {
            for (int i = 0; i < n; i++) {
                bool bit = (order[i].first >> b) & 1;
                pref[b][i + 1] = pref[b][i] + bit;
                sum[b][i + 1] = sum[b][i] + (bit ? order[i].first : 0);
            }

            int zeroes = 0, ones = mid[b] = n - pref[b][n];
            for (int i = 0; i < n; i++) temp[((order[i].first >> b) & 1) ? ones++ : zeroes++] = order[i];
            order.swap(temp);
        }
    }

    tuple<T, T, int, int, int> quantile(int l, int r, int k) {
        T v = 0, s = 0;
        for (int b = lg - 1; ~b; b--) {
            int zeroes = (r - pref[b][r]) - (l - pref[b][l]);
            if (k >= zeroes) {
                k -= zeroes;
                v |= ((T) 1) << b;
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                s += sum[b][r] - sum[b][l];
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        s += (r - l - k) * v;
        return {v, s, k, r - l, order[l + k].second};
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

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

template <typename T>
vector<pair<T, int>> smawk(int n, int m, auto &&get, auto &&cmp) {
    int lg = __lg(n);
    vector<pair<T, int>> dp(n);
    vector<int> cols(n), offset(lg + 1, 0);
    for (int b = 0; b < lg; b++) {
        int size = 0;
        auto push = [&](int col, int limit = 0) {
            int temp = size;
            for (; size > limit; size--) {
                int row = (size << (b + 1)) - 1;
                pair<T, int> p{get(row, col), col};
                if (!cmp(dp[row], p)) break;
                dp[row] = p;
            }
            if (size == n >> (b + 1)) return;

            if (size == temp) {
                int row = ((size + 1) << (b + 1)) - 1;
                dp[row] = {get(row, col), col};
            }
            cols[offset[b] + size++] = col;
        };

        if (!b)
            for (int col = 0; col < m; col++) push(col);
        else
            for (int i = offset[b - 1]; i < offset[b]; i++) push(cols[i], (i - offset[b - 1]) >> 1);
        offset[b + 1] = offset[b] + size;
    }

    for (int b = lg; b; b--)
        for (int row = (1 << b) - 1, i = offset[b - 1]; row < n; row += 2 << b) {
            int stop = row + (1 << b) < n ? dp[row + (1 << b)].second : -1, col = cols[i];
            dp[row] = {get(row, col), col};
            if (col == stop) continue;
            for (i++; i < offset[b]; i++) {
                col = cols[i];
                pair<T, int> p{get(row, col), col};
                if (cmp(dp[row], p)) dp[row] = p;
                if (col == stop) break;
            }
        }

    for (int row = 0, col = 0; row < n; row += 2) {
        int stop = row + 1 < n ? dp[row + 1].second : -1;
        dp[row] = {get(row, col), col};
        if (col == stop) continue;
        for (col++; col < m; col++) {
            pair<T, int> p{get(row, col), col};
            if (cmp(dp[row], p)) dp[row] = p;
            if (col == stop) break;
        }
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;

        pref[i + 1] = pref[i] + c;
    }

    vector<long long> S(n);
    for (auto &si : S) cin >> si;

    WaveletMatrix wm(n, S);
    int m = n - k + 1;
    auto get = [&](int row, int col) {
        if (row < col) return LLONG_MIN;
        int l = m - 1 - row, r = m - 1 - col + k;
        auto [v, sum, _, __, ___] = wm.quantile(l, r, r - l - k);
        return sum - pref[r] + pref[l];
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1 < p2;
    };
    auto rows = smawk<long long>(m, m, get, cmp);

    auto profit = LLONG_MIN;
    for (auto [value, col] : rows) profit = max(profit, value);
    cout << profit << "\n";

    vector<pair<int, int>> candidates;
    for (int row = m - 1; ~row; row--)
        if (rows[row].first == profit) candidates.emplace_back(m - 1 - row, m - 1 - rows[row].second);
    candidates.emplace_back(-1, m - 1);

    vector<tuple<long long, int, int>> intervals;
    for (int i = 0; i + 1 < candidates.size(); i++) {
        for (int l = candidates[i].first, j = candidates[i].second; j <= candidates[i + 1].second; j++) {
            int r = j + k;
            auto [v, sum, _, __, ___] = wm.quantile(l, r, r - l - k);
            if (sum - pref[r] + pref[l] == profit) intervals.emplace_back(v, l, r);
        }
    }
    sort(intervals.begin(), intervals.end());

    string s(n, '0');
    DisjointSets dsu(n + 1);
    vector<int> vals(n, -1), order(n + 1);
    iota(order.begin(), order.end(), 0);
    for (auto [v, l, r] : intervals)
        for (int i = order[dsu.find(l)]; i < r; i = order[dsu.find(i)]) {
            s[i] += S[i] >= v;
            auto [big, small] = dsu.unite(i, i + 1);
            order[big] = max(order[big], order[small]);
        }
    cout << s;
}