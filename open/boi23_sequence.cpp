#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(const vector<int> &v) const {
        auto h = 0ULL;
        for (int e: v) h ^= e + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> divisors(n + 1);
    for (int i = 2; i <= n; i++)
        for (int j = i * i; j <= n; j += i) divisors[j].emplace_back(i);

    vector<int> w(n + 1);
    for (int i = 1; i <= n; i++) cin >> w[i];

    cout << w[1] << "\n";
    if (n >= 2) cout << w[1] + w[2] << "\n";
    if (n <= 2) exit(0);

    unordered_map<vector<int>, int, Hash> memo;
    auto dfs = [&](auto &&self, vector<int> curr) {
        if (curr.empty()) return w[1] + w[2];
        if (memo.find(curr) != memo.end()) return memo[curr];

        auto temp = curr;
        int x = temp.back();
        temp.pop_back();

        int dp = INT_MAX;
        for (int d : divisors[x]) {
            auto next = temp;
            int j = x / d;
            if (d > 2) next.emplace_back(d);
            if (j > 2) next.emplace_back(j);
            sort(next.begin(), next.end());
            next.erase(unique(next.begin(), next.end()), next.end());
            dp = min(dp, self(self, next));
        }
        if (x > 3) temp.emplace_back(x - 1);
        sort(temp.begin(), temp.end());
        temp.erase(unique(temp.begin(), temp.end()), temp.end());
        dp = min(dp, self(self, temp));
        return memo[curr] = dp + w[x];
    };

    for (int i = 3; i <= n; i++) cout << dfs(dfs, {i}) << "\n";
}
