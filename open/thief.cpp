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

    vector<vector<int>> jewels(301);
    while (n--) {
        int s, v;
        cin >> s >> v;

        jewels[s].emplace_back(v);
    }

    vector<long long> dp(k + 1, -1e18), temp(k + 1, -1e18);
    dp[0] = 0;
    for (int s = 1; s <= 300; s++) {
        int steal = min((int) jewels[s].size(), k / s);
        if (!steal) continue;

        sort(jewels[s].rbegin(), jewels[s].rend());
        vector<long long> pref(steal + 1, 0);
        for (int i = 0; i < steal; i++) pref[i + 1] = pref[i] + jewels[s][i];

        vector<long long> a(steal + 1);
        for (int i = 0; i <= steal; i++) a[i] = -pref[i];

        fill(temp.begin(), temp.end(), -1e18);
        for (int r = 0; r < s && r <= k; r++) {
            int size = (k - r) / s + 1;
            vector<long long> b(size);
            for (int i = 0; i < size; i++) b[i] = -dp[i * s + r];

            auto c = min_plus_convolution(a, b);
            for (int i = 0; i < size; i++) temp[i * s + r] = -c[i];
        }
        dp = temp;
    }

    for (int i = 1; i <= k; i++) {
        dp[i] = max(dp[i], dp[i - 1]);
        cout << dp[i] << " ";
    }
}
