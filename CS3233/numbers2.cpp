#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> matmul(vector<vector<int>> &a, vector<vector<int>> &b) {
    vector<vector<int>> c(2, vector<int>(2));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) c[i][j] = (a[k][j] * b[i][k] + c[i][j]) % 1000;

    return c;
}

vector<vector<int>> matpow(vector<vector<int>> a, int n) {
    vector<vector<int>> b(2, vector<int>(2, 0));
    for (int i = 0; i < 2; i++) b[i][i] = 1;
    while (n) {
        if (n & 1) b = matmul(a, b);
        a = matmul(a, a);
        n >>= 1;
    }

    return b;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;

        vector<vector<int>> a = {{3, 5}, {1, 3}};
        cout << "Case #" << x << ": " << setfill('0') << setw(3) << (2 * matpow(a, n)[0][0] - 1 + 1000) % 1000 << "\n";
    }
}
