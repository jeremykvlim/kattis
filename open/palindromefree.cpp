#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    cin >> a >> b;

    int size = 18 * 11 * 11 * 2;
    vector<long long> memo1(size, -1), memo2(size, -1);
    auto index = [&](int p, int x, int y, int less) {
        return ((p * 18 + x) * 11 + y) * 11 + less;
    };
    auto dp = [&](auto &&self, auto &memo, const string &s, int p = 0, int x = 10, int y = 10, bool less = true) -> long long {
        if (p == s.size()) return 1;
        int i = index(p, x, y, less);
        if (memo[i] != -1) return memo[i];

        memo[i] = 0;
        for (int d = 0; d <= (less ? s[p] - '0' : 9); d++)
            if (d != x && d != y)
                memo[i] += (!d && x == 10 && y == 10) ? self(self, memo, s, p + 1, x, y, false) : self(self, memo, s, p + 1, d, x, less && d == s[p] - '0');

        return memo[i];
    };

    if (a) a = dp(dp, memo1, to_string(a - 1));
    b = dp(dp, memo2, to_string(b));
    cout << b - a;
}
