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

    SuffixArray(string &s, int r = 128) : ascii(s.begin(), s.end()) {
        SA = sais(ascii, r);
    };
};

int duval(const string &s) {
    int n = s.size();

    auto ss = s + s;
    int min_rotation_index = 0;
    for (int i = 0; i < n;) {
        int j = i + 1, k = i;
        min_rotation_index = i;
        for (; j < 2 * n && ss[j] >= ss[k]; j++) {
            if (ss[j] > ss[k]) k = i;
            else k++;
        }
        for (; i <= k && i < n; i += j - k);
    }
    return min_rotation_index;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string s;
    cin >> n >> m >> s;

    if (n < 2 * m) {
        int i = duval(s);
        cout << (s.substr(i) + s.substr(0, i)).substr(0, m);
        exit(0);
    }

    s += s.substr(0, m);
    SuffixArray sa(s);
    auto lcp = sa.kasai();

    int l = 0;
    for (int most = 0, il = 0; il < lcp.size();) {
        int ir = il;
        for (; ir + 1 < lcp.size() && lcp[ir] >= m; ir++);

        vector<int> indices;
        for (int i = il; i <= ir; i++)
            if (sa[i] < n) indices.emplace_back(sa[i]);

        if (!indices.empty()) {
            if (!most) {
                most = 1;
                l = sa[il];
            }

            int k = indices.size();
            if (k > most) {
                sort(indices.begin(), indices.end());

                vector<int> cycle_indices(2 * k);
                for (int i = 0; i < k; i++) {
                    cycle_indices[i] = indices[i];
                    cycle_indices[i + k] = indices[i] + n;
                }

                int size = 2 * k + 1;
                vector<int> next(size, 2 * k);
                for (int i = 0; i < 2 * k; i++) {
                    int j = i + 1;
                    for (; j < 2 * k && cycle_indices[j] < cycle_indices[i] + m; j++);
                    next[i] = j;
                }

                int blocks = ceil(sqrt(size));
                vector<int> jump(size), block_step(size);
                for (int i = size - 1; ~i; i--)
                    if (i == 2 * k) {
                        jump[i] = i;
                        block_step[i] = 0;
                    } else {
                        int j = next[i];
                        if (i / blocks != j / blocks) {
                            jump[i] = j;
                            block_step[i] = 1;
                        } else {
                            jump[i] = jump[j];
                            block_step[i] = block_step[j] + 1;
                        }
                    }

                int count = 1;
                for (int i = 0; i < k; i++) {
                    int bound = i + 1;
                    for (; bound < 2 * k && cycle_indices[bound] <= cycle_indices[i] + n - m; bound++);

                    int c = 0, j = i;
                    while (j < bound && jump[j] < bound) {
                        c += block_step[j];
                        j = jump[j];
                    }
                    while (j < bound) {
                        c++;
                        j = next[j];
                    }

                    count = max(count, c);
                    if (count == n / m) {
                        cout << s.substr(sa[il], m);
                        exit(0);
                    }
                }

                if (most < count) {
                    most = count;
                    l = sa[il];
                }
            }
        }

        il = ir + 1;
    }
    cout << s.substr(l, m);
}
