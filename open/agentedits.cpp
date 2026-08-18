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

struct SuffixArray {
    string s;
    vector<int> SA, ascii, SA_inv, lcp;
    SparseTable<int> st;

    vector<int> sais(vector<int> &ascii1, int range) {
        int n = ascii1.size();
        if (!n) return {};
        if (n == 1) return {0};
        if (n == 2) return ascii1[0] < ascii1[1] ? vector<int>{0, 1} : vector<int>{1, 0};

        vector<int> sa(n, 0), sum_s(range + 1, 0), sum_l(range + 1, 0);
        vector<bool> sl(n, false);
        for (int i = n - 2; ~i; i--) sl[i] = ascii1[i] == ascii1[i + 1] ? sl[i + 1] : ascii1[i] < ascii1[i + 1];
        for (int i = 0; i < n; i++)
            if (!sl[i]) sum_s[ascii1[i]]++;
            else sum_l[ascii1[i] + 1]++;

        for (int i = 0; i <= range; i++) {
            sum_s[i] += sum_l[i];
            if (i < range) sum_l[i + 1] += sum_s[i];
        }

        auto induced_sort = [&](vector<int> &lms) {
            fill(sa.begin(), sa.end(), -1);
            vector<int> b(range + 1, 0);
            copy(sum_s.begin(), sum_s.end(), b.begin());
            for (int i : lms) sa[b[ascii1[i]]++] = i;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            sa[b[ascii1[n - 1]]++] = n - 1;
            for (int j : sa)
                if (j > 0 && !sl[j - 1]) sa[b[ascii1[j - 1]]++] = j - 1;

            copy(sum_l.begin(), sum_l.end(), b.begin());
            for (int i = n - 1; ~i; i--) {
                int j = sa[i];
                if (j > 0 && sl[j - 1]) sa[--b[ascii1[j - 1] + 1]] = j - 1;
            }
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
                    for (; l < l_end && ascii1[l] == ascii1[r]; l++, r++);
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
        st = SparseTable<int>(lcp, [](int x, int y) { return min(x, y); });
    }
};

struct PURQSegmentTree {
    struct Monoid {
        int value;

        Monoid() : value(1e9) {}

        auto & operator=(const int &v) {
            value = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            value = min(value, monoid.value);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string f;
    int q;
    cin >> f >> q;
    int n = f.size();

    vector<pair<int, int>> events;
    while (q--) {
        char e;
        cin >> e;

        if (e == '?') {
            int i, j;
            cin >> i >> j;

            events.emplace_back(i, j);
        } else {
            char c;
            cin >> c;

            events.emplace_back(0, 0);
            f += c;
        }
    }

    int m = f.size();
    PURQSegmentTree st(m);
    vector<int> lcs(m + 1), start(m + 1);
    auto update = [&](int i) {
        if (i > lcs[i]) {
            start[i] = i - lcs[i];
            st.point_update(i - 1, lcs[i] + 1);
        } else {
            start[i] = 0;
            st.point_update(i - 1, 1e9);
        }
    };

    reverse(f.begin(), f.end());
    SuffixArray sa(f);
    set<int> active;
    int r = 1;
    auto add = [&]() {
        int i = sa.SA_inv[m - r], longest = 0;
        auto it = active.lower_bound(i);
        auto check = [&](int j) {
            int l = m - sa[j], len = sa.substring_lcp(m - r, sa[j]);
            longest = max(longest, len);
            if (lcs[l] < len) {
                lcs[l] = len;
                update(l);
            }
        };
        if (it != active.begin()) check(*prev(it));
        if (it != active.end()) check(*it);
        lcs[r] = longest;
        update(r++);
        active.emplace_hint(it, i);
    };
    while (r <= n) add();
    for (auto [i, j] : events)
        if (!i && !j) add();
        else {
            int k = upper_bound(start.begin() + j, start.begin() + r, i) - start.begin() - 1;
            cout << min(k < r - 1 ? k - i + 2 : (int) 1e9, st.range_query(j - 1, k).value) << "\n";
        }
}
