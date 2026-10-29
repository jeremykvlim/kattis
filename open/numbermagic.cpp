#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long n;
    int q;
    cin >> n >> q;

    vector<unsigned long long> p10(20, 1), repunit(20, 0);
    for (int i = 1; i < 20; i++) {
        p10[i] = p10[i - 1] * 10;
        repunit[i] = repunit[i - 1] * 10 + 1;
    }

    auto digits = [&](auto k) -> int {
        return upper_bound(p10.begin(), p10.end(), k) - p10.begin();
    };

    unordered_set<unsigned long long> seen{n};
    vector<unsigned long long> curr{n}, next;
    auto add = [&](auto k) {
        if (seen.emplace(k).second) next.emplace_back(k);
    };
    for (int i = 0; i < 25; i++) {
        next.clear();
        for (auto k : curr) {
            add(k + repunit[digits(k)]);
            if (k > 1) add(k / 2);
        }
        curr.swap(next);
    }

    auto dfs = [&](auto &&self, auto k, int ops = 7) -> bool {
        if (seen.contains(k)) return true;
        if (!ops) return false;
        if (self(self, 2 * k, ops - 1) || self(self, 2 * k + 1, ops - 1)) return true;

        int d = digits(k);
        for (int i = max(d - 1, 1); i <= d; i++)
            if (k > repunit[i] && p10[i - 1] <= k - repunit[i] && k - repunit[i] < p10[i] && self(self, k - repunit[i], ops - 1)) return true;
        return false;
    };

    while (q--) {
        unsigned long long m;
        cin >> m;
        cout << (dfs(dfs, m) ? "YES\n" : "NO\n");
    }
}
