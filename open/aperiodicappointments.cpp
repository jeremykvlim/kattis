#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<long long> s{0};
    while (s.size() <= k + 1 && n >= s.back() * k + 1) s.emplace_back(s.back() * k + 1);

    int ones = 0;
    if (s.size() == k + 2 && n > s.back()) {
        ones += n - s.back();
        n = s.back();
    }

    for (int i = s.size() - 1; i; i--) {
        ones += n / s[i] * s[i - 1];
        n %= s[i];
    }

    cout << ones;
}
