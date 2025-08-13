#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, r, b;
    cin >> n >> p >> r >> b;

    auto base = (long long) n * r;

    if (!b) {
        cout << base;
        exit(0);
    }

    auto time = LLONG_MAX;
    for (int kl = 1; kl <= n; ) {
        int kr = n / (n / kl);
        for (int k : {kl, kr}) {
            int quo = n / k, rem = n - k * quo;

            auto t = base + (long long) b * k * quo * (quo - 1) / 2 + (long long) b * quo * rem + (long long) p * (k - 1);
            time = min(time, t);
        }
        kl = kr + 1;
    }
    cout << time;
}
