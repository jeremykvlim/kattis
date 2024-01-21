#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9;

vector<vector<long long>> matmul(vector<vector<long long>> &a, vector<vector<long long>> &b) {
    vector<vector<long long>> c(2, vector<long long>(2));
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) c[i][j] = (a[k][j] * b[i][k] + c[i][j]) % MODULO;

    return c;
}

vector<vector<long long>> matpow(vector<vector<long long>> a, long long n) {
    vector<vector<long long>> b(2, vector<long long>(2, 0));
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

    int p;
    cin >> p;

    while (p--) {
        long long k, y;
        cin >> k >> y;

        vector<vector<long long>> a = {{1, 1}, {1, 0}};
        cout << k << " " << matpow(a, y)[0][1] << "\n";
    }
}
