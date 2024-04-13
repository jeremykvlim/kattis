#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

vector<vector<long long>> I(int size) {
    vector<vector<long long>> I(size, vector<long long>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

vector<vector<long long>> matmul(vector<vector<long long>> &a, vector<vector<long long>> &b) {
    int r1 = a.size(), r2 = b.size(), c2 = b[0].size();

    vector<vector<long long>> c(r1, vector<long long>(c2, 0));
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            for (int k = 0; k < r2; k++) c[i][j] = (c[i][j] + (a[i][k] * b[k][j]) % MODULO) % MODULO;

    return c;
}

vector<vector<long long>> matpow(vector<vector<long long>> a, long long n) {
    int size = a.size();
    auto b = I(size);

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

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<vector<long long>> a1(50, vector<long long>(50, 0)), a2(50, vector<long long>(1, 0)),
                                  b1(50, vector<long long>(50, 0)), b2(50, vector<long long>(1, 0));
        for (int i = 1; i < 50; i++) a1[i][i - 1] = b1[i][i - 1] = 1;
        a2[0][0] = b2[0][0] = 1;

        while (k--) {
            int l;
            cin >> l;

            a1[0][l - 1]++;
            b1[0][l - 1] = 1;
        }

        a1 = matpow(a1, n);
        b1 = matpow(b1, n);
        a2 = matmul(a1, a2);
        b2 = matmul(b1, b2);

        if (b2[0][0]) cout << a2[0][0] << "\n";
        else cout << "IMPOSSIBLE\n";
    }
}
