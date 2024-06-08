#include <bits/stdc++.h>
using namespace std;

bool ref(vector<vector<bool>> &matrix) {
    int r = matrix.size(), c = matrix[0].size();
    auto temp = matrix;

    for (int i = 0; i < min(r, c); i++) {
        int pivot = find_if(temp.begin() + i, temp.end(), [i](auto r) {return r[i];}) - temp.begin();

        if (pivot == matrix.size()) return false;

        swap(matrix[i], matrix[pivot]);
        swap(temp[i], temp[pivot]);

        for (int j = i + 1; j < r; j++) {
            for (int k = i + 1; k < c; k++) temp[j][k] = temp[j][k] - temp[i][k] * temp[j][i] / temp[i][i];
            temp[j][i] = false;
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

    vector<vector<bool>> valid{vector<bool>(n, false)};
    for (int i = 0; i < 1 << n; i++) {
        vector<bool> bits(n, false);
        for (int j = 0; j < n; j++) bits[j] = (i >> j) & 1;

        if (P[count(bits.begin(), bits.end(), true)]) valid.emplace_back(bits);
    }

    if (!ref(valid)) {
        cout << "impossible";
        exit(0);
    }

    vector<vector<bool>> code{vector<bool>(n, false)};
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
