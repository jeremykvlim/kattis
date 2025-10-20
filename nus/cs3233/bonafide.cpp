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

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    string s;
    cin >> n >> Q >> s;
    n = 2 * n + 1;

    string t(n, '#');
    for (int i = 0; i < n; i++)
        if (i & 1) t[i] = s[i / 2];

    auto rev = t;
    reverse(rev.begin(), rev.end());
    auto both = t + '_' + rev;
    SuffixArray sa(both);

    vector<int> radius0(n), radius1(n), radius2(n), s0(n), s1(n), s2(n);
    for (int i = 0; i < n; i++) {
        int bound = min(i, n - 1 - i);
        auto matches = [&](int offset) -> int {
            int l = 0, r = bound - offset + 2, m;
            if (r <= 0) return 0;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (sa.substring_lcp(2 * n - (i - offset), i + offset) >= m) l = m;
                else r = m;
            }
            return l;
        };
        int m0 = matches(1);

        int m1 = m0;
        char mismatch1_l = '-', mismatch1_r = '-';
        if (m0 < bound) {
            mismatch1_l = t[i - m0 - 1];
            mismatch1_r = t[i + m0 + 1];
            m1 += matches(m0 + 2) + 1;
        }

        int m2 = m1;
        char mismatch2_l = '-', mismatch2_r = '-';
        if (m1 < bound) {
            mismatch2_l = t[i - m1 - 1];
            mismatch2_r = t[i + m1 + 1];
            m2 += matches(m1 + 2) + 1;
        }

        radius0[i] = m0 + 1;
        radius1[i] = m1 + 1;
        radius2[i] = m2 + 1;

        bool b0 = true, b1 = false, b2 = false;
        if (m0 < bound)
            if (t[i] == mismatch1_l || t[i] == mismatch1_r) b1 = true;

        if (m1 < bound)
            if ((mismatch1_l == mismatch2_l && mismatch1_r == mismatch2_r) || (mismatch1_l == mismatch2_r && mismatch1_r == mismatch2_l)) b2 = true;

        s0[i] = b0 - b1;
        s1[i] = b1 - b2;
        s2[i] = b2;
    }

    vector<pair<int, int>> queries(Q);
    for (auto &[l, r] : queries) {
        cin >> l >> r;
        l = 2 * (l - 1);
        r = 2 * (r - 1) + 2;
    }

    vector<long long> count(Q, 0);
    auto process = [&](const auto &active, int sgn) {
        vector<int> left(n), right(n);
        for (int i = 0; i < n; i++) {
            left[i] = active[i] - i;
            right[i] = active[i] + i;
        }

        auto sum = [&](int l, int r) {
            return (long long) (l + r) * (r - l + 1) / 2;
        };

        vector<array<long long, 5>> ql, qr;
        for (int i = 0; i < Q; i++) {
            auto [l, r] = queries[i];
            int m = l + (r - l) / 2;

            if (l <= m) ql.push_back({l, m, 1 - l, i, sum(l, m)});
            if (m + 1 <= r) qr.push_back({m + 1, r, r + 1, i, -sum(m + 1, r)});
        }

        auto sweep = [&](const auto &half, const auto &q) {
            vector<pair<int, int>> order(n);
            for (int i = 0; i < n; i++) order[i] = {half[i], i};
            sort(order.begin(), order.end());

            vector<int> indices(q.size());
            iota(indices.begin(), indices.end(), 0);
            sort(indices.begin(), indices.end(), [&](int i, int j) {
                if (q[i][2] != q[j][2]) return q[i][2] < q[j][2];
                return i < j;
            });

            FenwickTree<long long> fw_a(n + 1), fw_b(n + 1);
            for (int i = 0; i < n; i++) fw_b.update(i + 1, 1);

            int j = 0;
            for (int k : indices) {
                auto [l, r, c, i, base] = q[k];
                for (; j < n && order[j].first <= c; j++) {
                    int h = order[j].second;
                    fw_a.update(h + 1, half[h]);
                    fw_b.update(h + 1, -1);
                }
                count[i] += sgn * (fw_a.range_sum_query(l, r + 1) + c * (fw_b.range_sum_query(l, r + 1)) + base);
            }
        };
        sweep(left, ql);
        sweep(right, qr);

        vector<int> pref(n + 1);
        for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + (!(i & 1) && active[i] > 0);
        for (int i = 0; i < Q; i++) {
            auto [l, r] = queries[i];
            count[i] -= sgn * (pref[r + 1] - pref[l]);
        }
    };

    auto apply = [&](const vector<int> &radius, const auto &sign) {
        vector<int> active(n);
        auto eval = [&](int sgn) {
            for (int i = 0; i < n; i++) active[i] = sign[i] == sgn ? radius[i] : 0;
            process(active, sgn);
        };
        eval(1);
        eval(-1);
    };
    apply(radius0, s0);
    apply(radius1, s1);
    apply(radius2, s2);

    for (auto c : count) cout << c / 2 << "\n";
}
