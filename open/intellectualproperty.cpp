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

    SuffixArray(const vector<int> &a, int r = 128) : ascii(a.begin(), a.end()) {
        SA = sais(ascii, r);
    };
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool print = false;
    for (int k, n = 1; cin >> k && k; n++) {
        cin.ignore();
        if (print) cout << "\n";
        print = true;

        string s;
        getline(cin, s);

        string tdp;
        while (getline(cin, s)) {
            if (s == "END TDP CODEBASE") break;
            tdp += s + "\n";
        }

        getline(cin, s);

        string jcn;
        while (getline(cin, s)) {
            if (s == "END JCN CODEBASE") break;
            jcn += s + "\n";
        }

        vector<int> t, indices;
        for (int c : tdp) {
            t.emplace_back(c + 1);
            indices.emplace_back(1);
        }
        t.emplace_back(0);
        indices.emplace_back(0);

        for (int c : jcn) {
            t.emplace_back(c + 1);
            indices.emplace_back(0);
        }
        t.emplace_back(0);
        indices.emplace_back(0);

        int m = jcn.size(), offset = tdp.size() + 1;
        SuffixArray sa(t);
        auto lcp = sa.kasai();
        vector<int> left(m), right(m);
        for (int i = 0, prev = -1, l = INT_MAX; i < t.size(); i++) {
            if (i && ~prev) l = min(l, lcp[i - 1]);
            if (offset <= sa[i] && sa[i] < offset + m) left[sa[i] - offset] = ~prev ? l : 0;
            if (indices[sa[i]] > 0) {
                prev = i;
                l = INT_MAX;
            }
        }
        for (int i = t.size() - 1, prev = -1, l = INT_MAX; ~i; i--) {
            if (i < t.size() - 1 && ~prev) l = min(l, lcp[i]);
            if (offset <= sa[i] && sa[i] < offset + m) right[sa[i] - offset] = ~prev ? l : 0;
            if (indices[sa[i]] > 0) {
                prev = i;
                l = INT_MAX;
            }
        }

        vector<pair<int, int>> segments;
        for (int i = 0, r = -1; i < m; i++) {
            int l = min(m - i, max(left[i], right[i]));
            if (r < i + l && l) {
                r = i + l;
                segments.emplace_back(l, i);
            }
        }
        sort(segments.begin(), segments.end(), [&](const auto &p1, const auto &p2) { return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second; });

        cout << "CASE " << n << "\n";
        for (int i = 0; i < segments.size() && i < k; i++) {
            auto [l, p] = segments[i];
            cout << "INFRINGING SEGMENT " << i + 1 << " LENGTH " << l << " POSITION " << p << "\n" << jcn.substr(p, l) << "\n";
        }
    }
}
