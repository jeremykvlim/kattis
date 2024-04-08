#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int most = 0;
    auto total = 0LL;
    while (n--) {
        int a;
        cin >> a;

        most = max(most, a);
        total += a;
    }

    cout << max((long long) most * k - total, k - (total - 1) % k - 1);
}
