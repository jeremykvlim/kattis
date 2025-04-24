#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<long long> masks(n, 0);
    for (int day = 0; day < m; day++)
        for (int i = 1; i <= n; i++) {
            int x;
            cin >> x;

            if (2 * i > n) masks[x - 1] |= (1LL << day);
        }

    sort(masks.begin(), masks.end());
    for (int i = 1; i < n; i++)
        if (masks[i] == masks[i - 1]) {
            cout << "NO";
            exit(0);
        }
    cout << "YES";
}
