#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    vector<int> SA, ascii, SA_inv;

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

    vector<int> kasai() {
        int n = ascii.size();
        vector<int> lcp(n);
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
        return lcp;
    }

    int & operator[](int i) {
        return SA[i];
    }

    SuffixArray(vector<int> a, int r = 128) : ascii(a.begin(), a.end()) {
        SA = sais(ascii, r);
    };
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

    int R, C, x, y;
    cin >> R >> C >> x >> y;

    vector<string> amulet(R);
    for (auto &row : amulet) cin >> row;

    int delim = 256;
    vector<int> _1D, row_or_col, indices;
    for (int i = 0; i < R; i++) {
        for (char c : amulet[i]) {
            _1D.emplace_back((char) c);
            row_or_col.emplace_back(1);
            indices.emplace_back(i);
        }
        _1D.emplace_back(delim++);
        row_or_col.emplace_back(0);
        indices.emplace_back(-1);
    }
    for (int j = 0; j < C; j++) {
        for (int i = 0; i < R; i++) {
            _1D.emplace_back((char) amulet[i][j]);
            row_or_col.emplace_back(2);
            indices.emplace_back(j);
        }
        _1D.emplace_back(delim++);
        row_or_col.emplace_back(0);
        indices.emplace_back(-1);
    }
    int n = _1D.size();

    auto temp = _1D;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> s(n);
    for (int i = 0; i < n; i++) s[i] = lower_bound(temp.begin(), temp.end(), _1D[i]) - temp.begin() + 1;
    SuffixArray sa(s, temp.size() + 1);
    auto lcp = sa.kasai();

    vector<array<int, 3>> queries;
    stack<pair<int, int>> st;
    for (int r = 1; r <= n; r++) {
        int len = r < n ? lcp[r - 1] : 0, prev = r;
        while (!st.empty() && st.top().first > len) {
            auto [length, l] = st.top();
            st.pop();

            if (l <= r) queries.push_back({l - 1, r - 1, length});
            prev = l;
        }

        if (len && (st.empty() || st.top().first < len)) st.emplace(len, prev);
    }

    vector<int> len(n);
    for (int i = n - 1, prev = n; ~i; i--) {
        if (!~indices[i]) prev = i;
        len[i] = prev - i;
    }

    for (int i = 0; i < n; i++) {
        if (!len[sa[i]]) continue;
        int length = 0;
        if (i) length = max(length, lcp[i - 1]);
        if (i + 1 < n) length = max(length, lcp[i]);
        if (length < len[sa[i]]) queries.push_back({i, i, len[sa[i]]});
    }

    vector<vector<pair<int, int>>> sweep(n);
    for (auto [l, r, length] : queries) sweep[r].emplace_back(length, l);

    FenwickTree<int> fw_r(n + 1), fw_c(n + 1);
    vector<int> prev_r(R, -1), prev_c(C, -1);
    pair<int, int> longest{0, n + 1};
    auto &[code, i] = longest;
    for (int r = 0; r < n; r++) {
        int k = sa[r];
        if (row_or_col[k] == 1) {
            int id = indices[k];
            fw_r.update(r + 1, 1);
            if (~prev_r[id]) fw_r.update(prev_r[id] + 1, -1);
            prev_r[id] = r;
        } else if (row_or_col[k] == 2) {
            int id = indices[k];
            fw_c.update(r + 1, 1);
            if (~prev_c[id]) fw_c.update(prev_c[id] + 1, -1);
            prev_c[id] = r;
        }

        for (auto [length, l] : sweep[r])
            if (fw_r.pref_sum(r + 1) - fw_r.pref_sum(l) == x && fw_c.pref_sum(r + 1) - fw_c.pref_sum(l) == y) longest = max(longest, {length, -l});
    }

    if (!code) cout << -1;
    else
        for (int j = 0; j < code; j++) cout << (char) _1D[sa[-i] + j];
}
