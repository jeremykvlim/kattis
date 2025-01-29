#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "0";
        exit(0);
    }

    vector<long long> a(n);
    for (auto &ai : a) cin >> ai;

    auto s = LLONG_MIN;
    for (int i = 1; i < n; i++) s = max(s, (a[i] & ~a[i - 1]) ^ (((a[i - 1] ^ a[i]) + 1) / 2));
    cout << s;
}
