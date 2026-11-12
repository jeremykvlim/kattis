#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> d(n), dx(n), dy(n);
    double x = 0, y = 0;
    int count = 0;
    for (int i = 0; i < n; i++) {
        double e, f;
        cin >> d[i] >> e >> f;

        auto a = d[i] * M_PI / 180, s = sin(a), c = cos(a);
        dx[i] = (f - e) * s;
        dy[i] = (f - e) * c;

        auto w = d[i] < 180 ? f : e;
        x += w * s;
        y += w * c;
        if (d[i] < 180) count++;
    }

    auto h = x * x + y * y;
    for (int sgn : {-1, 1})
        for (int i = count - 1, j = n - 1; ~i || j >= count;) {
            bool b = j < count || (~i && d[i] + 180 >= d[j]);
            int k = b ? i-- : j--, s = b ? sgn : -sgn;
            x += s * dx[k];
            y += s * dy[k];
            h = max(h, x * x + y * y);
        }
    cout << fixed << setprecision(6) << sqrt(h);
}
