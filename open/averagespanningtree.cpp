#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

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

template <typename T>
Matrix<T> rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;
    auto matrix_inv = I<T>(n);

    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (matrix[i][c] > matrix[pivot][c]) pivot = i;

        if (!matrix[pivot][c]) continue;
        swap(matrix[pivot], matrix[rank]);
        swap(matrix_inv[pivot], matrix_inv[rank]);

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) {
            matrix[rank][j] *= temp;
            matrix_inv[rank][j] *= temp;
        }

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) {
                    matrix[i][j] -= temp * matrix[rank][j];
                    matrix_inv[i][j] -= temp * matrix_inv[rank][j];
                }
            }

        rank++;
    }

    return matrix_inv;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int v, e;
    cin >> v >> e;

    vector<array<int, 3>> edges(e);
    Matrix<double> laplacian(v - 1);
    for (auto &[a, b, w] : edges) {
        cin >> a >> b >> w;
        a--;
        b--;

        if (a != v - 1) laplacian[a][a]++;
        if (b != v - 1) laplacian[b][b]++;

        if (a != v - 1 && b != v - 1) {
            laplacian[a][b]--;
            laplacian[b][a]--;
        }
    }

    auto inv = rref(laplacian);
    auto average = 0.;
    for (auto [a, b, w] : edges) average += w * (a == v - 1 ? inv[b][b] : (b == v - 1 ? inv[a][a] : inv[a][a] + inv[b][b] - 2 * inv[a][b]));
    cout << fixed << setprecision(5) << average;
}