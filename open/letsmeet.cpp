#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    friend auto operator*(Matrix<T> &A, Matrix<T> &B) {
        int r1 = A.r, r2 = B.r, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] += A[i][k] * B[k][j];

        return C;
    }

    friend auto operator*=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
void rref(Matrix<T> &matrix) {
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
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false)), meet(n, vector<bool>(n, false));
    vector<int> degree(n, 0);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_matrix[u][v] = adj_matrix[v][u] = true;
        degree[u]++;
        degree[v]++;
    }

    int s, t;
    cin >> s >> t;

    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) {
        meet[i][i] = true;
        q.emplace(i, i);
    }

    while (!q.empty()) {
        auto [u, v] = q.front();
        q.pop();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (adj_matrix[u][i] && adj_matrix[v][j] && !meet[i][j]) {
                    meet[i][j] = true;
                    q.emplace(i, j);
                }
    }

    if (!meet[s][t]) {
        cout << "never meet";
        exit(0);
    }

    Matrix<double> A(n * n + n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j || !meet[i][j]) {
                A[i * n + j][i * n + j] = 1;
                continue;
            }

            A[i * n + j][i * n + j] = A[i * n + j][n * n] = degree[i] * degree[j];
            for (int k = 0; k < n; k++)
                for (int l = 0; l < n; l++)
                    if (adj_matrix[i][k] && adj_matrix[j][l]) A[i * n + j][k * n + l] = -1;
        }

    rref(A);
    cout << fixed << setprecision(6) << A[s * n + t][n * n];
}
