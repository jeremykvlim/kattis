#include <bits/stdc++.h>
using namespace std;

struct SuffixArray {
    vector<int> SA, ascii, SA_inv;

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

    vector<int> inv() {
        SA_inv.resize(SA.size());
        for (int i = 0; i < SA.size(); i++) SA_inv[SA[i]] = i;
        return SA_inv;
    }

    int & operator[](int i) {
        return SA[i];
    }

    SuffixArray(vector<int> a, int r = 128) : ascii(a.begin(), a.end()) {
        SA = sais(ascii, r);
    };
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> seqs(n);
    vector<int> all;
    for (int i = 0; i < n; i++) {
        int l;
        cin >> l;

        seqs[i].resize(l);
        for (int &e : seqs[i]) {
            cin >> e;

            all.emplace_back(e);
        }
        all.emplace_back(1e9);
    }

    auto sorted = all;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    unordered_map<int, int> indices;
    for (int i = 0; i < sorted.size(); i++) indices[sorted[i]] = i;

    vector<int> str(all.size());
    for (int i = 0; i < all.size(); i++) str[i] = indices[all[i]];

    SuffixArray sa(str, sorted.size());
    auto sa_inv = sa.inv();

    vector<int> start(n, 0), pos(n, 0);
    for (int i = 0, j = 0; i < n; i++) {
        start[i] = j;
        j += seqs[i].size() + 1;
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < n; i++) pq.emplace(sa_inv[start[i]], i);
    while (!pq.empty()) {
        auto [_, i] = pq.top();
        pq.pop();

        cout << seqs[i][pos[i]++] << " ";
        if (pos[i] < seqs[i].size()) pq.emplace(sa_inv[start[i] + pos[i]], i);
    }
}