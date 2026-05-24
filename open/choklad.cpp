#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> k(n);
    vector<pair<long long, int>> odd_count;
    for (int i = 0; i < n; i++) {
        long long R, C;
        cin >> R >> C >> k[i];

        vector<long long> s;
        for (;;) {
            if (R == 1 && C == 1) {
                s.emplace_back(1);
                break;
            }

            if (R < C) swap(R, C);

            if (!(C & 1)) {
                C >>= 1;
                s.emplace_back(R * C);
            } else if (!(R & 1)) {
                R >>= 1;
                s.emplace_back(R * C);
            } else {
                s.emplace_back(((R * C) - C) / 2);
                ++R >>= 1;
            }
        }

        for (int l = 0, r = 1; l < s.size(); l = r++) {
            for (; r < s.size() && s[l] == s[r]; r++);
            if ((r - l) & 1) odd_count.emplace_back(s[l], i);
        }
    }
    sort(odd_count.rbegin(), odd_count.rend());

    vector<long long> odd_count_all;
    vector<vector<int>> indices(n);
    for (int l = 0, r = 1; l < odd_count.size(); l = r++) {
        for (; r < odd_count.size() && odd_count[l].first == odd_count[r].first; r++);

        odd_count_all.emplace_back(odd_count[l].first);
        for (int i = l; i < r; i++) indices[odd_count[i].second].emplace_back(odd_count_all.size() - 1);
    }

    int m = odd_count_all.size();
    vector<bool> parity(m, false);
    for (int i = 0; i < n; i++)
        if (k[i] & 1)
            for (int j : indices[i]) parity[j] = !parity[j];

    vector<long long> pref_sum(m + 1, 0);
    vector<bool> pref_parity(m + 1, false);
    bool p = false;
    for (int i = 0; i < m; i++) {
        pref_parity[i] = p;
        pref_sum[i + 1] = pref_sum[i];
        if (parity[i]) {
            pref_sum[i + 1] += (p ? -odd_count_all[i] : odd_count_all[i]);
            p = !p;
        }
    }
    pref_parity[m] = p;

    auto sum = [&](int l, int r, bool p = false) -> long long {
        if (l >= r) return 0;
        auto s = pref_sum[r] - pref_sum[l];
        return pref_parity[l] == p ? s : -s;
    };
    auto v = llabs(sum(0, m));
    for (int i = 0; i < n; i++)
        if (!indices[i].empty()) {
            auto s = 0LL;
            p = false;
            int l = 0;
            for (int r : indices[i]) {
                s += sum(l, r, p);
                p ^= pref_parity[l] ^ pref_parity[r];
                if (!parity[r]) {
                    s += (p ? -odd_count_all[r] : odd_count_all[r]);
                    p = !p;
                }
                l = r + 1;
            }
            v = min(v, llabs(s += sum(l, m, p)));
        }
    cout << v;
}
