#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<T> min_plus_convolution(const vector<T> &convex, const vector<T> &arbitrary) {
    if (arbitrary.empty() || convex.empty()) return {0};

    int n = arbitrary.size() + convex.size() - 1, m = arbitrary.size(), lg = __lg(n) + 1;
    if (n == 1) return {arbitrary[0] + convex[0]};

    vector<int> indices(n);
    auto cmp = [&] (int i, int j, int k) {
        if (i < k) return false;
        if (i - j >= convex.size()) return true;
        return arbitrary[j] + convex[i - j] >= arbitrary[k] + convex[i - k];
    };

    vector<vector<int>> candidates(lg);
    candidates[0].resize(m);
    iota(candidates[0].begin(), candidates[0].end(), 0);
    for (int b = 0; b < lg; b++) {
        vector<int> temp{candidates[b][0]};
        for (int i = 1; i < candidates[b].size(); i++) {
            int j = candidates[b][i], s = temp.size();
            for (; !temp.empty() && cmp((s - 1) << b, temp.back(), j); s = temp.size()) temp.pop_back();
            if (s << b < n) temp.emplace_back(j);
        }
        swap(candidates[b], temp);

        if (b + 1 < lg) candidates[b + 1] = candidates[b];
        else {
            indices[0] = candidates[b][0];
            if (candidates[b].size() > 1 && cmp(0, candidates[b][0], candidates[b][1])) indices[0] = candidates[b][1];
        }
    }

    for (int b = lg - 1, p2 = 1 << b; ~b; b--, p2 >>= 1)
        for (int i = p2, j = 0; i < n; i += p2 << 1, j--)
            for (int r = i + p2 >= n ? m - 1 : indices[i + p2]; j < candidates[b].size() && candidates[b][j] <= r; j++)
                if (cmp(i, indices[i], candidates[b][j])) indices[i] = candidates[b][j];

    vector<T> c(n);
    for (int i = 0; i < n; i++) c[i] = arbitrary[indices[i]] + convex[i - indices[i]];
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<char, int>> cards(n);
    for (auto &[c, v] : cards) cin >> c >> v;

    vector<int> muls, add;
    vector<vector<int>> adds;
    for (auto [c, v] : cards)
        if (c == 'a') add.emplace_back(v);
        else {
            adds.emplace_back(add);
            add.clear();
            muls.emplace_back(v);
        }
    adds.emplace_back(add);

    int m = muls.size();
    k = min(k, m);

    vector<long long> temp;
    vector<vector<long long>> dp(k + 1, {(long long) -1e18});
    dp[0][0] = 0;
    for (int block = 0; block <= m; block++) {
        if (!adds[block].empty()) {
            sort(adds[block].rbegin(), adds[block].rend());

            int s = adds[block].size();
            vector<long long> pref(s + 1, 0);
            for (int i = 0; i < s; i++) pref[i + 1] = pref[i] + adds[block][i];

            vector<long long> a(s + 1);
            for (int i = 0; i <= s; i++) a[i] = -pref[i];

            for (int i = 0; i <= min(block, k); i++) {
                vector<long long> b(dp[i].size());
                for (int j = 0; j < dp[i].size(); j++) b[j] = -dp[i][j];

                auto c = min_plus_convolution(a, b);
                temp.resize(min((int) c.size(), n - i + 1));
                for (int j = 0; j < temp.size(); j++) temp[j] = -c[j];
                dp[i] = temp;
            }
        }

        if (block < m && k)
            for (int i = min(block, k - 1); ~i; i--) {
                dp[i + 1].resize(min((int) dp[i].size(), n - i), -1e18);
                for (int j = 0; j < dp[i + 1].size(); j++)
                    if (dp[i][j] != -1e18) dp[i + 1][j] = max(dp[i + 1][j], dp[i][j] * muls[block]);
            }
    }

    vector<long long> score(n + 1, 0);
    for (int i = 0; i <= k; i++)
        for (int j = 0; j <= n - i && j < dp[i].size(); j++) score[i + j] = max(score[i + j], dp[i][j]);

    for (int i = 1; i <= n; i++) {
        score[i] = max(score[i], score[i - 1]);
        cout << score[i] << "\n";
    }
}
