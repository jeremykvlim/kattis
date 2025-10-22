#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    auto sum = 0LL;
    vector<long long> a(k);
    for (int i = 0; i < k; i++) {
        cin >> a[i];

        sum += a[i];
    }
    sort(a.begin(), a.end());

    int i = 0;
    auto s = 0LL;
    for (; i < k && s + 1 >= a[i]; i++) s += a[i];

    if (i == k) {
        cout << sum + 1;
        exit(0);
    }

    if (a[i] > s + 2) {
        cout << s + 1;
        exit(0);
    }

    for (int n = max(a[i], s + 2);; n <<= 1) {
        if (n == sum) {
            cout << sum;
            exit(0);
        }

        dynamic_bitset<> dp(n + 1, 1);
        for (int j = i; j < k && a[j] <= n;) {
            int aj = a[j], dupes = 0;
            for (; j < k && a[j] == aj && a[j] <= n; j++, dupes++);
            dupes = min(dupes, n / aj);

            for (int p2 = 1; dupes; p2 <<= 1) {
                int take = min(dupes, p2);
                dupes -= take;
                dp |= dp << (aj * take);
            }
        }

        int l = 2, r = s + 2, count = 0, pos = dp.find_first();
        while (pos < l) pos = dp.find_next(pos);
        for (; pos <= r; pos = dp.find_next(pos)) count++;
        for (; r != n; l++, r++) {
            if (!count) {
                cout << r - 1;
                exit(0);
            }

            count -= dp[l];
            count += dp[r + 1];
        }
    }
}
