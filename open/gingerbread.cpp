#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    int cookies = 0;
    auto dfs = [&](auto &&self, int remaining, int i = 0, int g = 0) {
        if (i == n) return g == 1;
        for (int c = 0; c <= remaining; c++)
            if (self(self, remaining - c, i + 1, __gcd(g, a[i] + c))) return true;
        return false;
    };
    while (!dfs(dfs, cookies)) cookies++;
    cout << cookies;
}
