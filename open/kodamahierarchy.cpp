#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> kodamas(n);
    for (auto &[a, c] : kodamas) cin >> a >> c;
    sort(kodamas.begin(), kodamas.end(), [&](auto p1, auto p2) { return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second; });

    vector<array<int, 3>> keys(n);
    for (int i = 0; i < n; i++) {
        auto [a, c] = kodamas[i];
        keys[i] = {c, -a, -i};
    }
    auto temp = keys;
    sort(keys.begin(), keys.end());

    vector<int> rank(n + 1), indices(n + 1);
    for (int i = 0; i < n; i++) rank[i] = lower_bound(keys.begin(), keys.end(), temp[i]) - keys.begin();
    rank[n] = n;
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return rank[i] < rank[j]; });

    vector<int> dp(n + 1, n + 1);
    for (int j = n + 1; int i : indices)
        if (j > i) {
            j = i;
            dp[i] = i < n;
        }

    vector<int> head(n + 1), tail(n + 1), next(n + 1), prev(n + 1);
    auto dnc = [&](auto &&self, int l, int r, const vector<int> &indices) {
        if (l + 1 == r) return;

        int m = l + (r - l) / 2;
        vector<int> left, right;
        for (int i : indices)
            if (i < m) left.emplace_back(i);
            else right.emplace_back(i);
        self(self, l, m, left);

        deque<int> mono_dec, mono_inc;
        for (int i : indices) {
            if (i < m) {
                while (!mono_dec.empty() && mono_dec.back() < i) mono_dec.pop_back();
                mono_dec.emplace_back(i);
            } else {
                reverse(mono_dec.begin(), mono_dec.end());
                head[i] = prev[i] = -1;
                if (!mono_dec.empty()) {
                    head[i] = prev[i] = mono_dec.back();
                    tail[prev[i]] = next[prev[i]] = -1;
                    mono_dec.pop_back();

                    while (!mono_dec.empty()) {
                        int j = mono_dec.back();
                        mono_dec.pop_back();

                        if (dp[j] < dp[head[i]]) {
                            next[j] = head[i];
                            head[i] = j;
                            tail[next[j]] = j;
                            tail[j] = -1;
                        }
                    }
                }

                while (!mono_inc.empty() && mono_inc.back() > i) {
                    int j = mono_inc.back();
                    mono_inc.pop_back();

                    int v = head[j];
                    while (~v && v < prev[i]) v = next[v];
                    if (~v) {
                        while (~prev[i] && dp[v] <= dp[prev[i]]) prev[i] = tail[prev[i]];
                        if (~prev[i]) {
                            tail[v] = prev[i];
                            next[prev[i]] = v;
                        } else {
                            head[i] = v;
                            tail[v] = -1;
                        }
                        prev[i] = prev[j];
                    }
                }

                if (~head[i]) dp[i] = min(dp[i], dp[head[i]] + (i < n));
                mono_inc.emplace_back(i);
            }
        }
        self(self, m, r, right);
    };
    dnc(dnc, 0, n + 1, indices);
    cout << dp[n];
}
