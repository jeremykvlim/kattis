#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b, c, i, k;
    while (cin >> a >> b >> c >> i >> k) {
        bool possible = false;
        for (int j = (i + c - 1) / c; j <= i; j++) possible |= (a * j + 1 <= k && k <= (a + b - 1) * j + 1);
        cout << (possible ? "possible\n" : "impossible\n");
    }
}
