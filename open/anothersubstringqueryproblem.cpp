#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    string s;
    vector<int> SA, ascii, SA_inv, lcp, lcp_l, lcp_r;

    vector<int> sais(vector<int> &ascii1, int range) {
        int n = ascii1.size();
        if (!n) return {};
        if (n == 1) return {0};
        if (n == 2) return ascii1[0] < ascii1[1] ? vector<int>{0, 1} : vector<int>{1, 0};

        vector<int> sa(n, 0), sum_s(range + 1, 0), sum_l(range + 1, 0);
        vector<bool> sl(n, false);
        for (int i = n - 2; ~i; i--) sl[i] = (ascii1[i] == ascii1[i + 1]) ? sl[i + 1] : (ascii1[i] < ascii1[i + 1]);
        for (int i = 0; i < n; i++) {
            if (!sl[i]) sum_s[ascii1[i]]++;
            else sum_l[ascii1[i] + 1]++;
        }

        for (int i = 0; i <= range; i++) {
            sum_s[i] += sum_l[i];
            if (i < range) sum_l[i + 1] += sum_s[i];
        }

        auto induced_sort = [&](vector<int> &lms) {
            fill(sa.begin(), sa.end(), -1);
            vector<int> b(range + 1, 0);
            copy(sum_s.begin(), sum_s.end(), b.begin());
            for (int i : lms)
                if (i < n) sa[b[ascii1[i]]++] = i;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            sa[b[ascii1[n - 1]]++] = n - 1;
            for (int j : sa)
                if (j > 0 && !sl[j - 1]) sa[b[ascii1[j - 1]]++] = j - 1;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            for (int i = n - 1, j = sa[i]; ~i; j = sa[--i])
                if (j > 0 && sl[j - 1]) sa[--b[ascii1[j - 1] + 1]] = j - 1;
        };

        vector<int> lms_map(n + 1, -1), lms;
        int m = 0;
        for (int i = 1; i < n; i++)
            if (!sl[i - 1] && sl[i]) {
                lms_map[i] = m++;
                lms.emplace_back(i);
            }
        induced_sort(lms);

        if (m) {
            vector<int> lms_sorted, ascii2(m);
            for (int j : sa)
                if (lms_map[j] != -1) lms_sorted.emplace_back(j);

            int range2 = 0;
            ascii2[lms_map[lms_sorted[0]]] = 0;
            for (int i = 1; i < m; i++) {
                int l = lms_sorted[i - 1], r = lms_sorted[i], l_end = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n, r_end = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
                bool same = true;
                if (l_end - l != r_end - r) same = false;
                else {
                    while (l < l_end && ascii1[l] == ascii1[r]) {
                        l++;
                        r++;
                    }

                    if (l == n || ascii1[l] != ascii1[r]) same = false;
                }

                if (!same) range2++;
                ascii2[lms_map[lms_sorted[i]]] = range2;
            }

            auto sa2 = sais(ascii2, range2);
            for (int i = 0; i < m; i++) lms_sorted[i] = lms[sa2[i]];
            induced_sort(lms_sorted);
        }

        return sa;
    }

    void kasai() {
        int n = ascii.size();
        lcp.resize(n);
        SA_inv.resize(n);
        for (int i = 0; i < n; i++) SA_inv[SA[i]] = i;
        for (int i = 0, k = 0; i < n; i++) {
            if (k) k--;
            if (!SA_inv[i]) continue;

            int j = SA[SA_inv[i] - 1];
            while (i + k < n && j + k < n && ascii[i + k] == ascii[j + k]) k++;
            lcp[SA_inv[i] - 1] = k;
        }
        lcp.back() = n;
    }

    int lcp_lr(int l, int r) {
        if (l + 1 >= r) return lcp[l];

        int m = l + (r - l) / 2;
        lcp_l[m] = lcp_lr(l, m);
        lcp_r[m] = lcp_lr(m, r);
        return min(lcp_l[m], lcp_r[m]);
    }

    int & operator[](int i) {
        return SA[i];
    }

    pair<int, int> matches(string &t) {
        auto search = [&](bool left) -> int {
            auto match = [&](auto t_l, auto s_l) {
                return mismatch(t.begin() + t_l, t.end(), s.begin() + s_l + t_l, s.end()).first - t.begin();
            };
            int match_l = match(0, SA[0]), match_r = match(0, SA[s.size() - 1]), match_m;

            if (left && match_l == t.size()) return 0;
            if (!left && match_r == t.size()) return s.size();

            int l = 0, r = s.size() - 1, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (match_l >= match_r) match_m = lcp_l[m] < match_l ? lcp_l[m] : match(match_l, SA[m]);
                else match_m = lcp_r[m] < match_r ? lcp_r[m] : match(match_r, SA[m]);

                if ((match_m != t.size() && SA[m] + match_m < s.size() && t[match_m] < s[SA[m] + match_m]) || (left && match_m == t.size())) {
                    r = m;
                    match_r = match_m;
                } else {
                    l = m;
                    match_l = match_m;
                }
            }
            return max(match_l, match_r) < t.size() ? s.size() : r;
        };
        return {search(true), search(false)};
    }

    SuffixArray(string &s, int r = 128) : s(s), ascii(s.begin(), s.end()), lcp_l(s.size()), lcp_r(s.size()) {
        SA = sais(ascii, r);
        kasai();
        lcp_lr(0, s.size() - 1);
    }
};

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end())) + 1), order(n) {
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

    string s;
    int q;
    cin >> s >> q;

    SuffixArray sa(s);
    WaveletMatrix wm(s.size(), sa.SA);
    while (q--) {
        string t;
        int k;
        cin >> t >> k;

        if (t.size() > s.size()) {
            cout << "-1\n";
            continue;
        }

        auto [l, r] = sa.matches(t);
        if (r - l < k || s[sa[r - 1] + t.size() - 1] != t.back()) {
            cout << "-1\n";
            continue;
        }

        cout << get<0>(wm.quantile(l, r, k - 1)) + 1 << "\n";
    }
}
