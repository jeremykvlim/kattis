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

    int n, t;
    cin >> n >> t;

    int sum_l = 0;
    vector<vector<int>> guards(6);
    for (int i = 0; i < n; i++) {
        int l, g;
        cin >> l >> g;

        guards[l].emplace_back(g);
        sum_l += l;
    }

    vector<long long> dp(sum_l + 1, 1e18);
    dp[0] = 0;
    for (int l = 1; l <= 5; l++)
        if (!guards[l].empty()) {
            sort(guards[l].begin(), guards[l].end());
            int m = guards[l].size();
            vector<long long> temp(sum_l + 1, 1e18), convex(m + 1, 0);
            for (int i = 0; i < m; i++) convex[i + 1] = convex[i] + guards[l][i];

            for (int k = 0; k < l; k++) {
                vector<long long> arbitrary;
                for (int i = k; i <= sum_l; i += l) arbitrary.emplace_back(dp[i]);
                if (arbitrary.empty()) break;
                auto c = min_plus_convolution(convex, arbitrary);
                for (int i = k, j = 0; i <= sum_l && j < c.size(); i += l, j++) temp[i] = min(temp[i], c[j]);
            }
            dp = temp;
        }

    int riches = 0;
    for (int i = sum_l; ~i; i--)
        if (dp[i] <= t) {
            riches = i;
            break;
        }
    cout << riches;
}
