#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << 1;
        exit(0);
    }

    vector<vector<int>> m(n, vector<int>(n));
    for (int l = 0; l < n; l++)
        for (int r = l; r < n; r++) cin >> m[l][r];

    int l = 0, r = n - 1;
    for (; m[l][n - 1] == m[l + 1][n - 1]; l++);
    for (; m[0][r - 1] == m[0][r]; r--);

    cout << l + 1 << " ";
    for (int i = l + 1; i <= r - 1; i++)
        if (m[l][i - 1] > m[l + 1][i - 1]) {
            if (m[l][i - 1] < m[l][i]) cout << i + 1 << " ";
        } else if (m[i + 1][r - 1] < m[i + 1][r]) {
            if (m[i][r] > m[i + 1][r]) cout << i + 1 << " ";
        } else if (m[l + 1][i - 1] == m[l + 1][i] && m[i][r - 1] == m[i + 1][r - 1]) cout << i + 1 << " ";
    cout << r + 1;
}
