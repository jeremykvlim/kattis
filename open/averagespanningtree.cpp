#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, int v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
T rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    T det = 1;
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);
        if (pivot != rank) det *= -1;

        det *= matrix[rank][c];
        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    return rank < n ? 0 : det;
}

template <typename T>
T kirchoffs_theorem(int n, const vector<pair<int, int>> &edges) {
    vector<int> degree(n, 0);
    for (auto [u, v] : edges) {
        degree[u]++;
        degree[v]++;
    }

    Matrix<T> laplacian(n - 1);
    for (int i = 0; i < n - 1; i++) laplacian[i][i] = degree[i];
    for (auto [u, v] : edges)
        if (u != n - 1 && v != n - 1) {
            laplacian[u][v]--;
            laplacian[v][u]--;
        }

    return rref(laplacian);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int v, e;
    cin >> v >> e;

    vector<pair<int, int>> edges(e);
    vector<int> weights(e);
    for (int i = 0; i < e; i++) {
        int a, b, w;
        cin >> a >> b >> w;

        edges[i] = {a - 1, b - 1};
        weights[i] = w;
    }

    double total = 0;
    for (int i = 0; i < e; i++) {
        vector<pair<int, int>> temp;
        auto [a1, b1] = edges[i];
        for (int j = 0; j < e; j++)
            if (i != j) {
                auto [a2, b2] = edges[j];
                if (b2 == b1) b2 = a1;
                if (a2 == b1) a2 = a1;
                temp.emplace_back(a2 - (a2 > b1), b2 - (b2 > b1));
            }

        total += weights[i] * kirchoffs_theorem<double>(v - 1, temp);
    }
    cout << fixed << setprecision(5) << total / kirchoffs_theorem<double>(v, edges);
}
