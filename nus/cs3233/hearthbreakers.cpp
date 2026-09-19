#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto sum = 0LL;
    vector<int> S(n);
    for (int &si : S) {
        cin >> si;

        sum += si;
    }
    sort(S.begin(), S.end());

    int biggest = S.back();
    if (biggest == 1) {
        cout << n;
        exit(0);
    }

    vector<int> distinct, freq;
    for (int i = 0, j = 1; i < n; i = j++) {
        for (; j < n && S[i] == S[j]; j++);
        distinct.emplace_back(S[i]);
        freq.emplace_back(j - i);
    }

    auto f = [&](int size) {
        int i = lower_bound(distinct.begin(), distinct.end(), size) - distinct.begin();
        return i < distinct.size() && distinct[i] == size ? freq[i] : 0;
    };
    int one = f(1), two = f(2), three = f(3);
    n -= one;

    auto calc = [&](int q) {
        for (auto d = 1LL; q * d <= 1e18; d *= 2) {
            int rocks = n;
            long long l = max(0LL, sum - q * d), r = min(d - 1, (long long) 1e18 - q * d), total = 0, used = 0;
            for (int x = 1;; x *= 2) {
                int a = q / x, b = q % x;
                if (a >= 3) {
                    int fl = f(a), fr = f(a + 1);
                    l = max(l, fr - b * d + total - a * used);
                    r = min(r, (x - b) * d + total - (a + 1) * used - fl);
                    if (l > r) goto next;
                    rocks -= fl + fr;
                    total += (long long) a * fl + (long long) (a + 1) * fr;
                    used = 2 * (used + fl + fr);
                } else if (a == 2) {
                    l = max(l, three - b * d + total - 2 * used);
                    if (two + three > x * d - used) goto next;
                    rocks -= two + three;
                    break;
                } else {
                    l = max(l, two - b * d + total - used);
                    rocks -= two;
                    break;
                }
            }

            if (l <= r && !rocks) return q * d + l;
            next:;
        }
        return LLONG_MAX;
    };
    auto m = min(calc(biggest), calc(biggest - 1));
    if (m == LLONG_MAX) cout << "IMPOSSIBLE";
    else cout << m;
}
