#include <bits/stdc++.h>
using namespace std;

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

    int n, k, b, m;
    cin >> n >> k >> b >> m;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto s = a;
    sort(s.begin(), s.end());
    s.erase(unique(s.begin(), s.end()), s.end());

    vector<vector<int>> pos(s.size() + 1);
    for (int i = 1; i <= n; i++) pos[a[i - 1] = lower_bound(s.begin(), s.end(), a[i - 1]) - s.begin() + 1].emplace_back(i);
    WaveletMatrix<int> wm(n, a);

    auto dfs = [&](auto &&self, int l = 0, int h = 0, int ways = 0, const vector<int> &dp1 = {}, const vector<int> &dp2 = {}) -> void {
        if (ways) {
            k -= ways;
            while (ways--) cout << h << "\n";
            if (!k) exit(0);
        }
        if (l >= n) return;

        for (int ai = 1; ai <= s.size(); ai++) {
            ai = wm.lower_bound(l, n, ai);
            if (ai == -1) return;

            int total = 0;
            vector<int> temp1, temp2;
            if (dp1.empty() && dp2.empty()) {
                total = min(k, (int) pos[ai].size());
                for (int i = 0; i < total; i++) {
                    temp1.emplace_back(pos[ai][i]);
                    temp2.emplace_back(1);
                }
            } else {
                int t = 0;
                for (int i = upper_bound(pos[ai].begin(), pos[ai].end(), l) - pos[ai].begin(), j = 0; i < pos[ai].size(); i++) {
                    for (; j < dp1.size() && dp1[j] < pos[ai][i]; j++) t = min(k, t + dp2[j]);
                    if (t) {
                        temp1.emplace_back(pos[ai][i]);
                        temp2.emplace_back(t);
                        total += t;
                        if (total >= k) break;
                    }
                }
            }
            if (total) self(self, *lower_bound(pos[ai].begin(), pos[ai].end(), l + 1), ((long long) h * b + s[ai - 1]) % m, min(total, k), temp1, temp2);
        }
    };
    dfs(dfs);
}
