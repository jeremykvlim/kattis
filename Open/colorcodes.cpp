#include <bits/stdc++.h>
using namespace std;

bool ref(int n, vector<vector<bool>> &valid) {
    auto matrix = valid;
    for (int i = 0; i < n; i++) {
        int d = find_if(matrix.begin() + i, matrix.end(), [i](auto r) {return r[i];}) - matrix.begin();

        if (d == matrix.size()) return false;

        swap(matrix[d], matrix[i]);
        swap(valid[d], valid[i]);

        for (int j = i + 1; j < matrix.size(); j++) {
            for (int k = i + 1; k <= n; k++) matrix[j][k] = matrix[j][k] - matrix[i][k] * matrix[j][i] / matrix[i][i];
            matrix[j][i] = false;
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;

    vector<bool> P(n + 1, false);
    while (p--) {
        int s;
        cin >> s;

        P[s] = true;
    }

    vector<vector<bool>> valid = {vector<bool>(n, false)};
    for (int i = 0; i < 1 << n; i++) {
        vector<bool> bits(n, false);
        for (int j = 0; j < n; j++) bits[j] = (i >> j) & 1;

        if (P[count(bits.begin(), bits.end(), true)]) valid.emplace_back(bits);
    }

    if (!ref(n, valid)) {
        cout << "impossible";
        exit(0);
    }

    vector<vector<bool>> code = {vector<bool>(n, false)};
    valid.resize(n);
    for (auto m : valid)
        for (int i = code.size() - 1; ~i; i--) {
            vector<bool> x(n);
            for (int j = 0; j < n; j++) x[j] = code[i][j] ^ m[j];
            code.emplace_back(x);
        }

    for (auto &x : code) {
        for (bool b : x) cout << b;
        cout << "\n";
    }
}
