#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<pair<int, int>> modes(t);
    for (auto &[dmin, a] : modes) cin >> dmin >> a;
    sort(modes.begin(), modes.end(), [&](auto p1, auto p2) { return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second; });

    vector<pair<int, int>> candidates;
    for (auto [dmin, a] : modes)
        if (candidates.empty() || candidates.back().second < a) candidates.emplace_back(dmin, a);

    int max_degree = 360000;
    auto normalize = [&](int x) {
        return (x + max_degree) % max_degree;
    };
    vector<long long> pref(n, 0);
    vector<int> angle(n - 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int d, h;
        cin >> d >> h;

        pref[i + 1] = pref[i] + d;
        if (i) angle[i] = normalize(angle[i - 1] + h);
    }

    int k = candidates.size();
    vector<vector<int>> right(k, vector<int>(n - 1, 0));
    vector<int> count(max_degree, 0);
    for (int i = 0; i < k; i++) {
        set<int> s;
        int d = max_degree - candidates[i].second, valid = 0;
        auto dist = [&](int x) {
            return (x + max_degree - 1) % max_degree + 1;
        };

        auto add = [&](int a) {
            if (count[a]++) return;

            if (s.empty()) {
                s.emplace(a);
                valid = 1;
                return;
            }

            auto it = s.lower_bound(a);
            int b = it == s.end() ? *s.begin() : *it, c = it == s.begin() ? *s.rbegin() : *prev(it);
            valid -= dist(b - c) >= d;
            valid += dist(a - c) >= d;
            valid += dist(b - a) >= d;
            s.emplace_hint(it, a);
        };

        auto remove = [&](int a) {
            if (--count[a]) return;

            if (s.size() == 1) {
                s.clear();
                valid = 0;
                return;
            }

            auto it = s.find(a);
            int b = it == prev(s.end()) ? *s.begin() : *next(it), c = it == s.begin() ? *s.rbegin() : *prev(it);
            valid -= dist(a - c) >= d;
            valid -= dist(b - a) >= d;
            valid += dist(b - c) >= d;
            s.erase(it);
        };

        for (int l = 0, r = 0; l < n - 1; l++) {
            for (; r < n - 1; r++) {
                add(angle[r]);
                if (!valid) {
                    remove(angle[r]);
                    break;
                }
            }

            right[i][l] = r + 1;
            remove(angle[l]);
        }
    }

    vector<int> dp(n, n), left(k, n);
    vector<deque<int>> dqs(k);
    dp[n - 1] = 0;
    for (int l = n - 2; ~l; l--)
        for (int i = 0; i < k; i++) {
            int r = right[i][l];

            while (!dqs[i].empty() && dqs[i].front() >= r) dqs[i].pop_front();
            while (left[i] - 1 > l && pref[left[i] - 1] - pref[l] >= candidates[i].first) {
                int j = --left[i];
                if (j >= r || dp[j] == n) continue;

                while (!dqs[i].empty() && dp[dqs[i].back()] >= dp[j]) dqs[i].pop_back();
                dqs[i].emplace_back(j);
            }

            if (!dqs[i].empty()) dp[l] = min(dp[l], dp[dqs[i].front()] + 1);
        }

    if (dp[0] == n) cout << "IMPOSSIBLE";
    else cout << dp[0];
}
