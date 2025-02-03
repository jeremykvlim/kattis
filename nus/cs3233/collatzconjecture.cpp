#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> a(n + 1);
    unordered_set<long long> f;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        f.emplace(a[i]);
    }

    while (n != 1) {
        for (int i = 1; i < n; i++) {
            a[i] = __gcd(a[i], a[i + 1]);
            f.emplace(a[i]);
        }
        n = unique(a.begin() + 1, a.begin() + n) - a.begin() - 1;
    }
    cout << f.size();
}
