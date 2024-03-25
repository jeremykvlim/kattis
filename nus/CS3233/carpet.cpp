#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double a, b, c;
    cin >> a >> b >> c;

    auto s = (a + b + c) / 2, area = sqrt(s * (s - a) * (s - b) * (s - c)), t = a * a + b * b + c * c, carpet = (sqrt(3) / 4 * t + 3 * area) / 2;
    cout << fixed << setprecision(3) << ((a + b >= c && a + c >= b && b + c >= a && sqrt(carpet * 4 / sqrt(3)) >= max({a, b, c})) ? carpet : -1);
}
