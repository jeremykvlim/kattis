#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e3;

vector<vector<int>> I(int size) {
    vector<vector<int>> I(size, vector<int>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

vector<vector<int>> matmul(vector<vector<int>> &a, vector<vector<int>> &b) {
    int r1 = a.size(), r2 = b.size(), c2 = b[0].size();

    vector<vector<int>> c(r1, vector<int>(c2, 0));
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            for (int k = 0; k < r2; k++) c[i][j] = (c[i][j] + (a[i][k] * b[k][j]) % MODULO) % MODULO;

    return c;
}

vector<vector<int>> matpow(vector<vector<int>> a, int n) {
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

    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;

        vector<vector<int>> a{{3, 5}, {1, 3}};
        cout << "Case #" << x << ": " << setfill('0') << setw(3) << (2 * matpow(a, n)[0][0] - 1 + MODULO) % MODULO << "\n";
    }
}
