#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    int all = 1, src = 2;
    vector<int> p3(10, 1);
    for (int d = 0; d <= 9; d++) {
        all *= 3;
        if (d) src += 2 * (p3[d] = p3[d - 1] * 3);
    }

    vector<array<int, 10>> rem(all);
    for (int i = 0; i < all; i++)
        for (int d = 0, temp = i; d <= 9; d++, temp /= 3) rem[i][d] = temp % 3;

    vector<array<int, 10>> count;
    vector<int> states, valid;
    for (int i = 0; i < n; i++) {
        array<int, 10> c{};
        if (!a[i]) c[0] = 1;
        else
            for (int temp = a[i]; temp; temp /= 10) c[temp % 10]++;

        int s = 0;
        for (int d = 0; d <= 9; d++) {
            if (c[d] > 2) goto next;
            s += c[d] * p3[d];
        }
        count.emplace_back(c);
        states.emplace_back(s);
        valid.emplace_back(a[i]);
        next:;
    }

    int m = count.size();
    vector<vector<long long>> memo(m + 1, vector<long long>(all, -1));

    auto dfs = [&](auto &&self, int i, int s) -> long long {
        if (i == m) return 0;

        if (memo[i][s] != -1) return memo[i][s];
        memo[i][s] = self(self, i + 1, s);
        for (int d = 0; d <= 9; d++)
            if (rem[s][d] < count[i][d]) return memo[i][s];
        return memo[i][s] = max(memo[i][s], valid[i] + self(self, i + 1, s - states[i]));
    };
    cout << dfs(dfs, 0, src);
}
