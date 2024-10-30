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

    int & operator[](int i) {
        return SA[i];
    }

    SuffixArray(string &s, int r = 128) : ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
    };
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k;
        string s;
        cin >> k >> s;

        int n = s.length();
        SuffixArray sa(s);
        vector<int> indices(n);
        for (int i = 0; i < n; i++) indices[sa[i]] = i;

        vector<vector<int>> next(n + 1, vector<int>(26, -1));
        vector<int> same(n + 1, 1);
        for (int i = n - 1; ~i; i--) {
            for (int j = 0; j < 26; j++) next[i][j] = next[i + 1][j];
            next[i][s[i] - 'a'] = i;
            if (i < n - 1 && s[i] == s[i + 1]) same[i] += same[i + 1];
        }

        string str;
        int l = 0;
        for (char c = 'z'; c >= 'a' && l < n && k; c--) {
            while (l < n && s[l] == c) str += s[l++];

            vector<int> diff;
            for (int i = l + 1; i < n && next[i][c - 'a'] != -1; i++)
                if (s[i] == c && s[i - 1] != c) diff.emplace_back(i);
            if (diff.empty()) continue;
            sort(diff.begin(), diff.end(), [&](int i, int j) {return same[i] == same[j] ? i < j : same[j] < same[i];});

            int i = 0, last = 0;
            for (; i < diff.size() && k; i++, k--) {
                int j = diff[i];

                last = j;
                while (j < n && s[j] == c) str += s[j++];
                l = max(l, j);
            }

            int r = l;
            for (; i < diff.size(); i++) {
                int j = diff[i];
                if (j > l && same[j] == same[last] && indices[j + same[j] - 1] > indices[r - 1]) r = j + same[j];
            }
            l = r;
        }

        for (int i = l; i < n; i++) str += s[i];
        cout << str << "\n";
    }
}
