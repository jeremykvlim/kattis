#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
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

    auto begin() {
        return SA.begin();
    }

    auto end() {
        return SA.end();
    }

    auto & operator[](int i) {
        return SA[i];
    }

    SuffixArray(string &s, int r = 128) : ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
    };
};

template <typename T>
struct WaveletTree {
    vector<T> WT, temp;
    vector<int> pref1, pref2;
    int b;

    void range_order_statistic(vector<tuple<int, int, int>> &queries) {
        for (int bit = b; ~bit; bit--) {
            for (int i = 0; i < WT.size(); i++) {
                pref1[i + 1] = pref1[i] + !((WT[i] >> bit) & 1);
                pref2[i + 1] = pref2[i] + ((WT[i] >> bit) & 1);
            }

            int it1 = 0, it2 = pref1.back();
            for (int e : WT) temp[!((e >> bit) & 1) ? it1++ : it2++] = e;
            swap(WT, temp);

            for (auto &[l, r, k] : queries) {
                if (r - l < k || !k) continue;

                int K = pref1[r] - pref1[l];
                if (k <= K) {
                    l = pref1[l];
                    r = pref1[r];
                } else {
                    k -= K;
                    l = pref1.back() + pref2[l];
                    r = pref1.back() + pref2[r];
                }
            }
        }
    }

    auto operator[](int i) {
        return WT[i];
    }

    WaveletTree(vector<T> array, vector<tuple<int, int, int>> &queries) : WT(array.begin(), array.end()),
                                                                          temp(array.size()),
                                                                          pref1(array.size() + 1),
                                                                          pref2(array.size() + 1) {
        b = __lg(*max_element(array.begin(), array.end()));
        range_order_statistic(queries);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    SuffixArray sa(s);
    vector<tuple<int, int, int>> queries(q);
    for (auto &[l, r, k] : queries) {
        string t;
        cin >> t >> k;

        if (t.size() > s.size()) {
            k = 0;
            continue;
        }

        l = lower_bound(sa.begin(), sa.end(), 0, [&](int i, int) -> bool {return lexicographical_compare(s.begin() + i, s.end(), t.begin(), t.end());}) - sa.begin();
        r = lower_bound(sa.begin() + l, sa.end(), 0, [&](int i, int) -> bool {return mismatch(t.begin(), t.end(), s.begin() + i, s.end()).first == t.end();}) - sa.begin();

        if (r - l < k || s[sa[r - 1] + t.size() - 1] != t.back()) k = 0;
    }

    WaveletTree<int> wt(sa.SA, queries);
    for (auto [l, r, k] : queries) cout << (k ? wt[r - 1] + 1 : -1) << "\n";
}
