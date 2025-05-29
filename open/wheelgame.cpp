#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<int, int> memo;
    auto grundy = [&](auto &&self, int n) {
        if (n <= 1) return 0;
        if (n >= 87) n = 53 + (n - 53) % 34;
        if (memo.count(n)) return memo[n];

        vector<int> seen;
        for (int i = 0; i - 1 <= n - i - 2; i++) seen.emplace_back(self(self, i) ^ self(self, n - i - 2));
        sort(seen.begin(), seen.end());
        seen.erase(unique(seen.begin(), seen.end()), seen.end());

        int mex = 0;
        for (int m : seen) {
            if (mex != m) break;
            mex++;
        }
        return memo[n] = mex;
    };
    cout << (grundy(grundy, n - 1) ? "Destroy the robot before it is too late" : "The robot is your friend");
}
