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

void parse(string s, vector<int> &H, vector<int> &T) {
    for (int i = 0; i < s.size(); i++)
        for (int j = 0; j <= i; j++)
            if (!j || s.substr(0, j) == s.substr(i - j, j)) {
                if (s[j] == 'H') H[i] = j + 1;
                if (s[j] == 'T') T[i] = j + 1;
            }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string g, k;
    long double p;
    cin >> g >> k >> p;
    p *= 100;

    int n = g.size(), m = k.size();
    if (n >= m && g.substr(n - m, m) == k) {
        cout << 0;
        exit(0);
    }

    vector<int> H_g(n + 1), T_g(n + 1), H_k(m + 1), T_k(m + 1);
    parse(g, H_g, T_g);
    parse(k, H_k, T_k);

    vector<vector<vector<pair<int, int>>>> adj_list(n + 1, vector<vector<pair<int, int>>>(m + 1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            adj_list[i][j].emplace_back(H_g[i], H_k[j]);
            adj_list[i][j].emplace_back(T_g[i], T_k[j]);
        }

    vector<vector<bool>> visited(n + 1, vector<bool>(m + 1, false));
    auto dfs = [&](auto &&self, int v = 0, int u = 0) -> void {
        visited[v][u] = true;
        for (auto [i, j] : adj_list[v][u])
            if (!visited[i][j]) self(self, i, j);
    };
    dfs(dfs);

    vector<vector<int>> indices(n + 1, vector<int>(m + 1, -1));
    int count = 0;
    for (int i = n; ~i; i--)
        for (int j = m; ~j; j--)
            if (visited[i][j]) indices[i][j] = count++;

    Matrix<long double> prob(count, count + 1);
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++) {
            if (indices[i][j] == -1) continue;

            prob[indices[i][j]][indices[i][j]] = 100;
            if (j == m) continue;
            if (i == n) {
                prob[indices[i][j]][count] = 100;
                continue;
            }

            prob[indices[i][j]][indices[H_g[i]][H_k[j]]] -= p;
            prob[indices[i][j]][indices[T_g[i]][T_k[j]]] -= 100 - p;
        }

    rref(prob);
    cout << fixed << setprecision(6) << prob[count - 1][count] / prob[count - 1][count - 1];
}
