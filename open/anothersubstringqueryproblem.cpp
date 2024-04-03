#include <bits/stdc++.h>
using namespace std;

vector<int> build(vector<int> &str, int ascii_range) {
    int n = str.size();

    vector<int> sa(n), lms_i;
    vector<bool> sl(n);
    sl[n - 1] = false;
    for (int i = n - 2; ~i; i--) {
        sl[i] = (str[i] > str[i + 1] || (str[i] == str[i + 1] && sl[i + 1]));
        if (sl[i] && !sl[i + 1]) lms_i.emplace_back(i + 1);
    }
    reverse(lms_i.begin(), lms_i.end());

    auto induced_sort = [&](vector<int> lms) -> void {
        vector<int> l(ascii_range, 0), r(ascii_range, 0);
        for (int c : str) {
            if (c + 1 < ascii_range) l[c + 1]++;
            r[c]++;
        }
        partial_sum(l.begin(), l.end(), l.begin());
        partial_sum(r.begin(), r.end(), r.begin());

        fill(sa.begin(), sa.end(), -1);
        for (int i = lms.size() - 1; ~i; i--) sa[--r[str[lms[i]]]] = lms[i];
        for (int i : sa)
            if (i && sl[i - 1]) sa[l[str[i - 1]]++] = i - 1;

        fill(r.begin(), r.end(), 0);
        for (int c : str) r[c]++;
        partial_sum(r.begin(), r.end(), r.begin());

        for (int k = sa.size() - 1, i = sa[k]; ~k; k--, i = sa[k])
            if (i && !sl[i - 1]) sa[--r[str[i - 1]]] = i - 1;
    };
    induced_sort(lms_i);

    vector<int> lms_j(lms_i.size()), lms_str(lms_i.size());
    for (int i = 0, j = 0; i < n; i++)
        if (!sl[sa[i]] && sa[i] && sl[sa[i] - 1]) lms_j[j++] = sa[i];

    int curr = 0;
    sa[n - 1] = curr;
    for (int k = 1; k < lms_j.size(); k++) {
        int i = lms_j[k - 1], j = lms_j[k];
        if (str[i] != str[j]) {
            sa[j] = ++curr;
            continue;
        }

        bool diff = false;
        for (int a = i + 1, b = j + 1;; a++, b++) {
            if (str[a] != str[b]) {
                diff = true;
                break;
            }

            if ((!sl[a] && sl[a - 1]) || (!sl[b] && sl[b - 1])) {
                diff = !((!sl[a] && sl[a - 1]) && (!sl[b] && sl[b - 1]));
                break;
            }
        }

        sa[j] = diff ? ++curr : curr;
    }

    for (int i = 0; i < lms_i.size(); i++) lms_str[i] = sa[lms_i[i]];
    if (curr + 1 < lms_i.size()) {
        auto lms_SA = build(lms_str, curr + 1);
        for (int i = 0; i < lms_i.size(); i++) lms_j[i] = lms_i[lms_SA[i]];
    }
    induced_sort(lms_j);

    return sa;
}
vector<int> suffix_array(string &s, int ascii_range = 128) {
    vector<int> str(s.begin(), s.end());
    str.emplace_back('$');

    auto sa = build(str, ascii_range);
    sa.erase(sa.begin());

    return sa;
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
    int Q;
    cin >> s >> Q;

    auto sa = suffix_array(s);
    unordered_map<pair<int, int>, vector<int>, Hash> occurrences;
    while (Q--) {
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

        if (last - first + 1 < k || s[sa[first] + t.size() - 1] != t.back()) {
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
