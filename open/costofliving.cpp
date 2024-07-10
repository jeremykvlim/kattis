#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    friend auto operator *(Matrix<T> &A, Matrix<T> &B) {
        int r1 = A.r, r2 = B.r, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] += A[i][k] * B[k][j];

        return C;
    }

    friend auto operator *=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    void add(vector<T> &row) {
        mat.emplace_back(row);
        r++;
    }
};

template <typename T>
vector<T> rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    vector<T> solution(m - 1, -1);
    for (int i = 0; i < n; i++) {
        int l = find_if(matrix[i].begin(), matrix[i].end(), [](auto value) {return fabs(value) > 1e-9;}) - matrix[i].begin(),
                r = find_if(matrix[i].rbegin() + 1, matrix[i].rend(), [](auto value) {return fabs(value) > 1e-9;}) - matrix[i].rbegin();

        if (l + 1 == m - r) solution[l] = exp(matrix[i][m - 1]);
    }

    return solution;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int y, c, q;
    cin >> y >> c >> q;

    vector<double> r(y - 1);
    for (auto &ri : r) cin >> ri;

    vector<vector<double>> p(y, vector<double>(c));
    for (auto &row : p)
        for (auto &pi : row) cin >> pi;

    Matrix<double> A((y - 1) * c, y + (y + 1) * c + 1);
    for (int i = 0; i < y; i++)
        for (int j = 0; j < c; j++) {
            if (i < y - 1) {
                A[i * c + j][i + j * y + 1] = -1;
                A[i * c + j][i + j * y] = A[i * c + j][i + (y + 1) * c] = A[i * c + j][y * c + j] = 1;
            }

            if (p[i][j] > 0) {
                vector<double> row(y + (y + 1) * c + 1);
                row[i + j * y] = 1;
                row.back() = log(p[i][j]);
                A.add(row);
            }
        }

    for (int i = 0; i < y - 1; i++)
        if (r[i] > 0) {
            vector<double> row(y + (y + 1) * c + 1);
            row[i + (y + 1) * c] = 1;
            row.back() = log(r[i]);
            A.add(row);
        }

    auto price = rref(A);
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << fixed << setprecision(4) << price[(a - 1) * y + b - 1] << "\n";
    }
}
