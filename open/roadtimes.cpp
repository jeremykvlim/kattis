#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    bool recalibrate;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)), recalibrate(false) {}

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

    void add() {
        vector<T> row(c, 0);
        mat.emplace_back(row);
        r++;
    }

    void add(vector<T> &row) {
        mat.emplace_back(row);
        r++;

        if (row.size() != c) {
            c = max(c, (int) row.size());
            recalibrate = true;
        }
    }

    void remove() {
        mat.pop_back();
        r--;
    }

    void adjust() {
        if (!recalibrate) return;
        for (auto &row : mat) row.resize(c);
    }
};

template <typename T>
void normalize_pivot_row(Matrix<T> &matrix, int p, T v) {
    int m = matrix.c;
    for (int i = 0; i < m; i++) matrix[p][i] /= v;
}

template <typename T>
void eliminate_pivot_column(Matrix<T> &matrix, int row, int p, T v) {
    int m = matrix.c;
    for (int i = 0; i < m; i++) matrix[row][i] -= v * matrix[p][i];
}

template <typename T>
void pivot(Matrix<T> &matrix, int pivot_col, int pivot_row) {
    int n = matrix.r;

    normalize_pivot_row(matrix, pivot_row, matrix[pivot_row][pivot_col]);

    for (int i = 0; i < n - 1; i++)
        if (i != pivot_row) eliminate_pivot_column(matrix, i, pivot_row, matrix[i][pivot_col]);
}

template <typename T>
T simplex(Matrix<T> &matrix, vector<int> &pivots) {
    T value = 0;
    int n = matrix.r, m = matrix.c;
    for (;;) {
        T least = 0;
        int pivot_col = -1;
        for (int i = 0; i < m - 1; i++)
            if (least < matrix[n - 1][i]) {
                least = matrix[n - 1][i];
                pivot_col = i;
            }
        if (pivot_col == -1) return value;

        T ratio = numeric_limits<T>::max();
        int pivot_row = -1;
        for (int i = 0; i < n - 1; i++)
            if (ratio > -matrix[i][m - 1] / matrix[i][pivot_col] && matrix[i][pivot_col] > 1e-9) {
                ratio = -matrix[i][m - 1] / matrix[i][pivot_col];
                pivot_row = i;
            }

        pivots[pivot_row] = pivot_col;
        pivot(matrix, pivot_col, pivot_row);

        T v = matrix[n - 1][pivot_col];
        value -= v * matrix[pivot_row][m - 1];
        eliminate_pivot_column(matrix, n - 1, pivot_row, v);
    }
}

template <typename T>
vector<int> rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    vector<int> pivots(n, -1);
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);
        pivots[rank] = c;

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    return pivots;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_matrix(n, vector<int>(n)), indices(n, vector<int>(n));
    int c = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            cin >> adj_matrix[i][j];

            if (adj_matrix[i][j] > 0) indices[i][j] = c++;
        }

    vector<int> prev(n);
    vector<int> dist(n);
    vector<bool> visited(n);
    auto dijkstra_dense = [&](int s) {
        fill(prev.begin(), prev.end(), -1);
        fill(dist.begin(), dist.end(), INT_MAX);
        dist[s] = 0;
        fill(visited.begin(), visited.end(), false);
        for (int i = 0; i < n; i++) {
            int d = INT_MAX, j = 0;
            for (int k = 0; k < n; k++)
                if (!visited[k] && d > dist[k]) {
                    d = dist[k];
                    j = k;
                }

            visited[j] = true;
            for (int k = 0; k < n; k++)
                if (!visited[k] && adj_matrix[j][k] > 0 && dist[k] > dist[j] + adj_matrix[j][k]) {
                    dist[k] = dist[j] + adj_matrix[j][k];
                    prev[k] = j;
                }
        }
    };

    int r;
    cin >> r;

    Matrix<double> A(r, c + 1);
    auto path = [&](int d, int s, auto &t, int row, bool rev = false) {
        while (d != s) {
            int v = prev[d];
            A[row][indices[v][d]] = rev ? -1 : 1;
            t += rev ? -adj_matrix[v][d] : adj_matrix[v][d];
            d = v;
        }
    };

    for (int i = 0; i < r; i++) {
        int s, d, t;
        cin >> s >> d >> t;

        dijkstra_dense(s);
        t *= -1;
        path(d, s, t, i);
        A[i][c] = t;
    }
    auto pivots = rref(A);

    while (pivots.back() == -1) {
        pivots.pop_back();
        A.remove();
        r--;
    }

    auto get_pivot = [&](int i) {
        int p = -1;
        for (int j = 0; j < pivots.size(); j++)
            if (pivots[j] == i) p = j;

        return p;
    };

    auto update = [&](int p, auto v, auto &t) {
        if (p == -1) return;
        for (int k = 0; k < c; k++) A[r - 1][k] -= v * A[p][k];
        t -= v * A[p].back();
    };

    for (int i = 0, count = c; i < n; i++)
        for (int j = 0; j < n; j++)
            if (adj_matrix[i][j] > 0) {
                pivots.emplace_back(count);
                vector<double> curr(count + 2, 0);
                curr[indices[i][j]] = curr[count++] = 1;
                curr[count] = -adj_matrix[i][j];
                A.add(curr);
                r++;

                update(get_pivot(indices[i][j]), A[r - 1][indices[i][j]], A[r - 1].back());
            }

    queue<double> temp;
    for (auto &row : A.mat) {
        temp.emplace(row.back());
        row.pop_back();
    }
    A.adjust();
    for (auto &row : A.mat) {
        row.back() = temp.front();
        temp.pop();
    }

    A.add();
    r++;
    c = A.c;
    double most = 0;
    int pivot_row = -1;
    for (int i = 0; i < r - 1; i++)
        if (most < A[i].back()) {
            most = A[i].back();
            pivot_row = i;
        }

    if (pivot_row != -1) {
        pivots[pivot_row] = -1;
        normalize_pivot_row(A, pivot_row, -1.);

        for (int i = 0; i < r - 1; i++)
            if (i != pivot_row) eliminate_pivot_column(A, i, pivot_row, -1.);
        eliminate_pivot_column(A, r - 1, pivot_row, -1.);
        simplex(A, pivots);

        pivot_row = get_pivot(-1);

        if (pivot_row != -1) {
            int pivot_col = -1;
            for (int i = 0; i < c - 1; i++)
                if (fabs(A[pivot_row][i]) > 1e-9) pivot_col = i;

            pivot(A, pivot_col, pivot_row);
            eliminate_pivot_column(A, r - 1, pivot_row, A[r - 1][pivot_col]);
        }
    }

    int q;
    cin >> q;

    while (q--) {
        int s, d;
        cin >> s >> d;

        dijkstra_dense(s);
        auto time = [&](int d, int s, bool rev = false) -> double {
            double t = 0;
            fill(A[r - 1].begin(), A[r - 1].end(), 0);
            path(d, s, t, r - 1, rev);
            for (int i = 0; i < c - 1; i++) update(get_pivot(i), A[r - 1][i], t);
            return t + simplex(A, pivots);
        };
        cout << fixed << setprecision(6) << s << " " << d << " " << -time(d, s, true) << " " << time(d, s) << "\n";
    }
}
