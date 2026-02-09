#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n + 1), r(n + 1), c(n + 1);
    for (int i = 1; i <= n; i++) cin >> p[i] >> r[i] >> c[i];
    r[0] = 101;

    int count = 0;
    for (int _ = 0; _ < n; _++) {
        int i = 1, j = 0;
        for (; i <= n; j += (i - j) * (r[i] < r[j]), i++);
        for (i = j; i && c[i] >= r[j]; i = p[i]);
        if (!i) {
            for (i = j; i; c[i] -= r[j], i = p[i]);
            count++;
        }
        r[j] = 101;
    }
    cout << count;
}
