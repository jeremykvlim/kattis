#include <bits/stdc++.h>
using namespace std;

vector<int> sais(vector<int> &str, int range) {
    int n = str.size();
    if (!n) return {};
    if (n == 1) return {0};
    if (n == 2) return str[0] < str[1] ? vector<int>{0, 1} : vector<int>{1, 0};

    vector<int> sa(n, 0), sum_s(range + 1, 0), sum_l(range + 1, 0);
    vector<bool> sl(n, false);
    for (int i = n - 2; ~i; i--) sl[i] = (str[i] == str[i + 1]) ? sl[i + 1] : (str[i] < str[i + 1]);
    for (int i = 0; i < n; i++) {
        if (!sl[i]) sum_s[str[i]]++;
        else sum_l[str[i] + 1]++;
    }

    for (int i = 0; i <= range; i++) {
        sum_s[i] += sum_l[i];
        if (i < range) sum_l[i + 1] += sum_s[i];
    }

    auto induced_sort = [&](vector<int> &lms) {
        fill(sa.begin(), sa.end(), -1);
        vector<int> b(range + 1, 0);
        copy(sum_s.begin(), sum_s.end(), b.begin());
        for (auto i : lms)
            if (i < n) sa[b[str[i]]++] = i;

        copy(sum_l.begin(), sum_l.end(), b.begin());
        sa[b[str[n - 1]]++] = n - 1;
        for (int j : sa)
            if (j > 0 && !sl[j - 1]) sa[b[str[j - 1]]++] = j - 1;

        copy(sum_l.begin(), sum_l.end(), b.begin());
        for (int i = n - 1, j = sa[i]; ~i; j = sa[--i])
            if (j > 0 && sl[j - 1]) sa[--b[str[j - 1] + 1]] = j - 1;
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
        vector<int> lms_sorted, str2(m);
        for (int j : sa)
            if (lms_map[j] != -1) lms_sorted.emplace_back(j);

        int range2 = 0;
        str2[lms_map[lms_sorted[0]]] = 0;
        for (int i = 1; i < m; i++) {
            int l = lms_sorted[i - 1], r = lms_sorted[i], l_end = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n, r_end = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
            bool same = true;
            if (l_end - l != r_end - r) same = false;
            else {
                while (l < l_end && str[l] == str[r]) {
                    l++;
                    r++;
                }

                if (l == n || str[l] != str[r]) same = false;
            }

            if (!same) range2++;
            str2[lms_map[lms_sorted[i]]] = range2;
        }

        auto sa2 = sais(str2, range2);
        for (int i = 0; i < m; i++) lms_sorted[i] = lms[sa2[i]];
        induced_sort(lms_sorted);
    }

    return sa;
}

vector<int> suffix_array(string &s, int ascii_range = 128) {
    vector<int> str(s.begin(), s.end());
    return sais(str, ascii_range);
}

struct Hash {
    uint64_t encode(const pair<int, int> &p) const {
        uint64_t encoded = 0;
        encoded = (encoded << 8) | p.first;
        encoded = (encoded << 8) | p.second;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        uint64_t hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const pair<int, int> &p) const {
        static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(p) + SEED);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    auto sa = suffix_array(s);
    unordered_map<pair<int, int>, vector<int>, Hash> occurrences;
    while (q--) {
        string t;
        int k;
        cin >> t >> k;

        if (t.size() > s.size()) {
            cout << "-1\n";
            continue;
        }

        int l = -1, r = s.size(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (s.compare(sa[m], t.size(), t) >= 0) r = m;
            else l = m;
        }
        int first = r;

        l = first, r = s.size();
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (s.compare(sa[m], t.size(), t)) r = m;
            else l = m;
        }
        int last = l;

        if (last - first + 1 < k || s[sa[last] + t.size() - 1] != t.back()) {
            cout << "-1\n";
            continue;
        }

        if (occurrences.count({first, last})) {
            cout << occurrences[{first, last}][k - 1] + 1 << "\n";
            continue;
        }

        vector<int> o(last - first + 1);
        for (int i = first; i <= last; i++) o[i - first] = sa[i];
        sort(o.begin(), o.end());
        occurrences[{first, last}] = o;
        cout << o[k - 1] + 1 << "\n";
    }
}
