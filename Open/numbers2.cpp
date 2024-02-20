#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e3;

vector<vector<int>> matmul(vector<vector<int>> &a, vector<vector<int>> &b) {
    vector<vector<int>> c(2, vector<int>(2, 0));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            c[i][j] = (a[0][j] * b[i][0] + a[1][j] * b[i][1]) % MODULO;

    return c;
}

vector<vector<int>> matpow(vector<vector<int>> a, int n) {
    vector<vector<int>> b{{1, 0}, {0, 1}};
    for (; n; n >>= 1) {
        if (n & 1) b = matmul(a, b);
        a = matmul(a, a);
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
        cout << "Case #" << x << ": " << setfill('0') << setw(3) << (2 * matpow(a, n)[0][0] - 1 + MODULO) % MODULO << "\n";
    }
}
