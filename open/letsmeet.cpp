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

    vector<vector<int>> adj_list(n);
    vector<int> degree(n, 0);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
        degree[u]++;
        degree[v]++;
    }

    int s, t;
    cin >> s >> t;

    if (s == t) {
        cout << 0;
        exit(0);
    }

    vector<vector<bool>> meet(n, vector<bool>(n, false));
    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) {
        meet[i][i] = true;
        q.emplace(i, i);
    }

    while (!q.empty()) {
        auto [u, v] = q.front();
        q.pop();

        for (int a : adj_list[u])
            for (int b : adj_list[v])
                if (!meet[a][b]) {
                    meet[a][b] = true;
                    q.emplace(a, b);
                }
    }

    if (!meet[s][t]) {
        cout << "never meet";
        exit(0);
    }

    vector<int> id(n * n, -1), U, V;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (meet[i][j]) {
                id[i * n + j] = U.size();
                U.emplace_back(i);
                V.emplace_back(j);
            }

    int k = U.size();
    Matrix<double> A(k, k + 1);
    for (int i = 0; i < k; i++) {
        int u = U[i], v = V[i];
        A[i][i] = A[i][k] = degree[u] * degree[v];
        for (int a : adj_list[u])
            for (int b : adj_list[v])
                if (a != b) {
                    auto [x, y] = minmax(a, b);
                    int j = id[x * n + y];
                    if (j != -1) A[i][j]--;
                }
    }
    rref(A);

    auto [x, y] = minmax(s, t);
    cout << fixed << setprecision(6) << A[id[x * n + y]][k];
}
