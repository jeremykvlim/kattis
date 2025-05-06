#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double v, h, s, g;
    cin >> v >> h >> s >> g;
    cout << (s * s * s * s + 2 * g * v * s * s >= g * g * h * h ? "YES" : "NO");
}
