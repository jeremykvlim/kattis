#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, k;
    cin >> n >> p >> k;

    auto total = 0LL;
    vector<int> d(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> d[i];

        total += d[i + n] = d[i];
    }

    auto base = p / total * k, sum = 0LL;
    p %= total;
    vector<int> next(n);
    vector<bool> extra(n, false);
    for (int i = 0, j = 0; i < n; sum -= d[i++]) {
        for (; j < 2 * n && sum + d[j] <= p; sum += d[j++]);
        next[i] = j;
        if (j >= n) {
            next[i] -= n;
            extra[i] = true;
        }
    }

    vector<int> prev(n, 0), step(n, -1);
    step[0] = k;
    int count = 0;
    for (int i = 0; k;) {
        count += extra[i];
        i = next[i];
        k--;
        if (~step[i]) {
            int len = step[i] - k;
            count += k / len * (count - prev[i]);
            k %= len;
        }
        step[i] = k;
        prev[i] = count;
    }
    cout << base + count;
}
