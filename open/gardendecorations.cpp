#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 1;

    return I;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w;
    cin >> w;

    if (!w) {
        cout << 3;
        exit(0);
    }

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    Matrix<int> A(n);
    for (int i = 0; i < n; i++) A[i][a[i]] = 1;

    auto decompose = [&]() -> vector<Matrix<int>> {
        auto L1_inv = I<int>(n), U = A, L2_inv = I<int>(n);
        for (int i = 0; i < n; i++) {
            if (!U[i][i])
                for (int j = i + 1; j < n; j++)
                    if (U[i][j]) {
                        for (int k = 0; k < n; k++) {
                            L1_inv[j][k] ^= L1_inv[i][k];
                            U[k][i] ^= U[k][j];
                        }
                        break;
                    }

            for (int j = i + 1; j < n; j++)
                if (U[j][i])
                    for (int k = i; k < n; k++) {
                        U[j][k] ^= U[i][k];
                        L2_inv[k][i] ^= L2_inv[k][j];
                    }
        }

        return {L1_inv, U, L2_inv};
    };
    
    auto M = decompose()[w - 1];
    vector<bool> b(n);
    for (int i = w & 1 ? 0 : n - 1; (w & 1 ? i < n : ~i); (w & 1 ? i++ : i--)) {
        bool bi;
        cin >> bi;

        b[i] = bi;
        bi = false;
        for (int j = w & 1 ? 0 : i; j <= (w & 1 ? i : n - 1); j++)
            if (M[i][j]) bi ^= b[j];

        cout << bi << "\n" << flush;
    }
}
