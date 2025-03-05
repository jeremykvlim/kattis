#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    SparseTable() {}
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
        int i = __lg(r - l);
        return f(ST[i][l], ST[i][r - (1 << i)]);
    }
};

struct SuffixArray {
    string s;
    vector<int> SA, ascii, SA_inv, lcp;
    SparseTable<int> st;

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

    int substring_lcp(int i, int j) {
        if (i == j) return s.size() - i;

        auto [l, r] = minmax(SA_inv[i], SA_inv[j]);
        return st.range_query(l, r);
    }

    int & operator[](int i) {
        return SA[i];
    }

    SuffixArray(string &s, int r = 128) : s(s), ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
        kasai();
        st = SparseTable<int>(lcp, [](int x, int y) {return min(x, y);});
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string x, y;
    cin >> x >> y;

    auto [it_x, it_y] = mismatch(x.rbegin(), x.rend(), y.rbegin(), y.rend());
    x.erase(it_x.base(), x.end());
    y.erase(it_y.base(), y.end());

    if (x.empty() && y.empty()) {
        cout << 1;
        exit(0);
    }

    int n = x.size();
    auto s = x + "#" + y;
    SuffixArray sa(s);

    auto equal = [&](int l1, int r1, int l2, int r2) -> bool {
        int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
        if (len1 != len2) return false;
        return sa.substring_lcp(l1, l2) >= len1;
    };

    vector<int> indices1, indices2;
    for (int i = 0; i < n; i++) {
        if (equal(0, i, n + n - i, n + n)) indices1.emplace_back(i);
        if (equal(n - 1 - i, n - 1, n + 1, n + 1 + i)) indices2.emplace_back(i);
    }
    reverse(indices1.begin(), indices1.end());
    reverse(indices2.begin(), indices2.end());
    if (indices1.size() > 2) indices1.resize(2);
    if (indices2.size() > 2) indices2.resize(2);

    for (int i : indices1)
        for (int j : indices2) {
            if (i > n - j - 2) continue;
            if (i == n - j - 2 || equal(i + 1, n - j - 2, n + j + 2, n + n - i - 1)) {
                cout << 1;
                exit(0);
            }
        }

    cout << 0;
}
