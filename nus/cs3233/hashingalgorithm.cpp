#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 998244353;

struct DisjointSet {
    vector<int> sets, size;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            size[p_set] += size[q_set];
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

struct SuffixArray {
    vector<int> SA, ascii, lcp;

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
        lcp.resize(n - 1);
        vector<int> rank(n);
        for (int i = 0; i < n; i++) rank[SA[i]] = i;
        for (int i = 0, k = 0; i < n; i++) {
            if (k) k--;
            if (!rank[i]) continue;

            int j = SA[rank[i] - 1];
            while (i + k < n && j + k < n && ascii[i + k] == ascii[j + k]) k++;
            lcp[rank[i] - 1] = k;
        }
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

    int c;
    cin >> c;

    while (c--) {
        string s;
        cin >> s;

        int n = s.size();
        SuffixArray sa(s);
        sa.kasai();
        
        vector<int> indices(n - 1), len(n);
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end(), [&](int i, int j) {return sa.lcp[i] > sa.lcp[j];});
        for (int i = 0; i < n; i++) len[i] = n - i;

        DisjointSet dsu(n);
        auto h = n == 1 ? 1LL : ((long long) n * n % MODULO * *min_element(sa.lcp.begin(), sa.lcp.end())) % MODULO;

        auto process = [&](auto &&self, int i = 0) -> void {
            if (i >= n - 1) return;

            auto add = [&](int i, int l) {
                int i_set = dsu.find(i), size = dsu.size[i_set];
                h += (len[i_set] - l) * ((long long) size * size % MODULO) % MODULO;
            };
            int l = sa.lcp[indices[i]];
            for (; i < n - 1 && sa.lcp[indices[i]] == l; i++) {
                int curr = sa[indices[i] + 1], prev = sa[indices[i]];
                add(curr, l);
                add(prev, l);

                dsu.unite(curr, prev);
                int curr_set = dsu.find(curr), prev_set = dsu.find(prev);
                len[curr_set] = min({len[curr_set], len[prev_set], l});
            }

            self(self, i);
        };

        process(process);
        cout << h % MODULO << "\n";
    }
}
