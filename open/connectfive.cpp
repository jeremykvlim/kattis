#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 5;
    vector<int> x(n), y(n);
    for (int i = 0; i < n; i++) cin >> x[i] >> y[i];

    int count = 0;
    while (n > 1) {
        auto check = [&](vector<int> &v) {
            vector<int> p(n);
            iota(p.begin(), p.end(), 0);
            sort(p.begin(), p.end(), [&](int i, int j) {return v[i] < v[j];});
            if (v[p[0]] != v[p[1]]) {
                count += v[p[1]] - v[p[0]];
                v[p[0]] = v[p[1]];
            }

            if (v[p[n - 2]] != v[p[n - 1]]) {
                count += v[p[n - 1]] - v[p[n - 2]];
                v[p[n - 1]] = v[p[n - 2]];
            }
        };
        check(x);
        check(y);

        bool change = false;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                if (x[i] == x[j] && y[i] == y[j]) {
                    x.erase(x.begin() + i);
                    y.erase(y.begin() + i);
                    n--;
                    change = true;
                }

        if (!change) break;
    }

    int xl = *min_element(x.begin(), x.end()), xr = *max_element(x.begin(), x.end()), yl = *min_element(y.begin(), y.end()), yr = *max_element(y.begin(), y.end());
    count += (1 + (yl < yr)) * (xr - xl) + (1 + (xl < xr)) * (yr - yl);
    for (int i = 0; i < n; i++)
        if (x[i] != xl && x[i] != xr && y[i] != yl && y[i] != yr) count += min(xr - xl, yr - yl);
    cout << count;
}
