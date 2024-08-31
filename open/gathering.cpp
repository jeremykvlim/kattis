#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    int d;
    cin >> d;

    auto up = LLONG_MIN, down = LLONG_MAX, left = LLONG_MIN, right = LLONG_MAX;
    for (int i = 0; i < n; i++) {
        up = max(up, x[i] + y[i] - d);
        down = min(down, x[i] + y[i] + d);
        left = max(left, x[i] - y[i] - d);
        right = min(right, x[i] - y[i] + d);
    }

    if (left > right || up > down) {
        cout << "impossible";
        exit(0);
    }

    nth_element(y.begin(), y.begin() + n / 2, y.end());

    auto travel = [&](long long X) {
        auto dist = 0LL;
        for (int xi : x) dist += abs(X - xi);

        auto yl = max(X - right, up - X), yr = min(X - left, down - X);
        if (yl > yr) return LLONG_MAX;

        auto Y = min(max(y[n / 2], yl), yr);
        for (int yi : y) dist += abs(Y - yi);

        return dist;
    };

    long long l = (left + up) / 2, r = (right + down) / 2, mid1, mid2;
    while (l + 2 < r) {
        mid1 = l + (r - l) / 3, mid2 = r - (r - l) / 3;

        if (travel(mid1) <= travel(mid2)) r = mid2;
        else l = mid1;
    }

    long long dist = LLONG_MAX;
    for (int i = l; i <= r; i++) dist = min(dist, travel(i));
    cout << dist;
}
