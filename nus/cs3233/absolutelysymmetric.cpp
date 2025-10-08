#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> a(n, vector<int>(n));
    for (auto &row : a)
        for (int &aij : row) cin >> aij;

    bool sym = true;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            if ((a[i][j] ^ a[j][i]) & 1) {
                cout << -1;
                exit(0);
            }

            if (abs(a[i][j]) != abs(a[j][i])) sym = false;
        }

    if (sym) {
        cout << "1\n";
        for (auto row : a) {
            for (int aij : row) cout << aij << " ";
            cout << "\n";
        }
    } else {
        cout << "2\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) cout << "0 ";
                else cout << (a[i][j] - a[j][i]) / 2 << " ";
            }
            cout << "\n";
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) cout << a[i][j] << " ";
                else cout << (a[i][j] + a[j][i]) / 2 << " ";
            }
            cout << "\n";
        }
    }
}
