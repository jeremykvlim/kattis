#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable(vector<T> v, function<T(T, T)> func) : f(move(func)) {
        if (v.empty()) return;
        int n = __lg(v.size()) + 1;
        ST.resize(n);
        ST.front() = v;
        for (int i = 1; i < n; i++) {
            ST[i].resize(v.size() - (1 << i) + 1);
            for (int j = 0; j <= v.size() - (1 << i); j++)
                ST[i][j] = f(ST[i - 1][j], ST[i - 1][j + (1 << (i - 1))]);
        }
    }

    T range_query(int l, int r) {
        int i = __lg(r - l + 1);
        return f(ST[i][l], ST[i][r - (1 << i) + 1]);
    }
};

struct SuffixArray {
    string s;
    vector<int> SA, ascii;

    vector<int> sais(vector<int> &ascii1, int range) {
        int n = ascii1.size();
        if (!n) return {};
        if (n == 1) return {0};
        if (n == 2) ascii1[0] < ascii1[1] ? vector<int>{0, 1} : vector<int>{1, 0};

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

    vector<int> kasai() {
        int n = ascii.size();
        vector<int> lcp(n), rank(n);
        for (int i = 0; i < n; i++) rank[SA[i]] = i;
        for (int i = 0, k = 0; i < n; i++) {
            if (k) k--;
            if (!rank[i]) continue;

            int j = SA[rank[i] - 1];
            while (i + k < n && j + k < n && ascii[i + k] == ascii[j + k]) k++;
            lcp[rank[i] - 1] = k;
        }
        lcp.back() = n;
        return lcp;
    }

    int & operator[](int i) {
        return SA[i];
    }

    pair<int, int> search(string &t, SparseTable<int> &lcp_st) {
        int match_l = mismatch(t.begin(), t.end(), s.begin() + SA.front(), s.end()).first - t.begin(),
            match_r = mismatch(t.begin(), t.end(), s.begin() + SA.back(), s.end()).first - t.begin(),
            match_m, l = 0, r = s.size(), m;

        auto update_match = [&]() {
            if (SA[m] + match_m < s.size() && s[SA[m] + match_m] > t[match_m]) {
                r = m;
                match_r = match_m;
            } else {
                l = m;
                match_l = match_m;
            }
        };

        auto match_lcp = [&](int m) -> pair<int, int> {
            auto search_half = [&](bool left) {
                int l = left ? -1 : m, r = left ? m : s.size();
                while (l + 1 < r) {
                    int mid = l + (r - l) / 2;

                    if (lcp_st.range_query(left ? mid : l, left ? r - 1 : mid - 1) < t.size()) (left ? l : r) = mid;
                    else (left ? r : l) = mid;
                }
                return r;
            };

            return {search_half(true), search_half(false)};
        };

        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (match_l < match_r) {
                int lcp = lcp_st.range_query(m, r - 1);

                if (match_r < lcp) r = m;
                else if (match_r > lcp) l = m;
                else {
                    match_m = mismatch(t.begin() + match_r, t.end(), s.begin() + SA[m] + match_r, s.end()).first - t.begin();

                    if (match_m != t.size()) update_match();
                    else return match_lcp(m);
                }
            } else {
                int lcp = lcp_st.range_query(l, m - 1);

                if (match_l < lcp) l = m;
                else if (match_l > lcp) r = m;
                else {
                    match_m = mismatch(t.begin() + match_l, t.end(), s.begin() + SA[m] + match_l, s.end()).first - t.begin();

                    if (match_m != t.size()) update_match();
                    else return match_lcp(m);
                }
            }
        }

        return {l, r};
    }

    SuffixArray(string &s, int r = 128) : s(s), ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
    };
};

template <typename T>
struct WaveletTree {
    vector<T> WT, temp;
    vector<int> pref_even, pref_odd;
    int b;

    void select(vector<array<int, 3>> &queries) {
        for (int bit = b; ~bit; bit--) {
            for (int i = 0; i < WT.size(); i++) {
                pref_even[i + 1] = pref_even[i] + !((WT[i] >> bit) & 1);
                pref_odd[i + 1] = pref_odd[i] + ((WT[i] >> bit) & 1);
            }

            int even = 0, odd = pref_even.back();
            for (int e : WT) temp[!((e >> bit) & 1) ? even++ : odd++] = e;
            swap(WT, temp);

            for (auto &[l, r, k] : queries) {
                if (r - l < k || !k) continue;

                int K = pref_even[r] - pref_even[l];
                if (k <= K) {
                    l = pref_even[l];
                    r = pref_even[r];
                } else {
                    k -= K;
                    l = pref_even.back() + pref_odd[l];
                    r = pref_even.back() + pref_odd[r];
                }
            }
        }
    }

    auto operator[](int i) {
        return WT[i];
    }

    WaveletTree(vector<T> a, vector<array<int, 3>> &k_order_statistics) : WT(a.begin(), a.end()), temp(a.size()),
                                                                          pref_even(a.size() + 1), pref_odd(a.size() + 1) {
        b = __lg(*max_element(a.begin(), a.end()));
        select(k_order_statistics);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    SuffixArray sa(s);
    SparseTable<int> st(sa.kasai(), [](int x, int y) {return min(x, y);});

    vector<array<int, 3>> k_order_statistics(q);
    for (auto &[l, r, k] : k_order_statistics) {
        string t;
        cin >> t >> k;

        if (t.size() > s.size()) {
            k = 0;
            continue;
        }

        tie(l, r) = sa.search(t, st);
        if (r - l < k || s[sa[r - 1] + t.size() - 1] != t.back()) k = 0;
    }

    WaveletTree<int> wt(sa.SA, k_order_statistics);
    for (auto [l, r, k] : k_order_statistics) cout << (k ? wt[r - 1] + 1 : -1) << "\n";
}
