#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    auto begin() {
        return mat.begin();
    }

    auto end() {
        return mat.end();
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
vector<int> rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    vector<int> match(m, -1);
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);
        match[c] = rank;

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    return match;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        Matrix<double> A(n, n + 1);
        for (auto &row : A)
            for (int j = 0; j < n; j++) cin >> row[j];
        for (auto &row : A) cin >> row[n];

        auto match = rref(A);
        for (auto row : A)
            if (none_of(row.begin(), row.begin() + n, [&](auto e) { return abs(e) > 1e-9; }) && abs(row[n]) > 1e-9) {
                cout << "inconsistent\n";
                goto next;
            }

        if (all_of(match.begin(), match.begin() + n, [&](int r) { return r != -1; }))
            for (int c = 0; c < n; c++) cout << fixed << setprecision(3) << A[match[c]][n] << " ";
        else {
            vector<int> cols;
            for (int c = 0; c < n; c++)
                if (match[c] == -1) cols.emplace_back(c);

            vector<string> x(n, "?");
            for (int c1 = 0; c1 < n; c1++)
                if (match[c1] != -1)
                    if (!any_of(cols.begin(), cols.end(), [&](int c2) { return abs(A[match[c1]][c2]) > 1e-9; })) x[c1] = to_string(A[match[c1]][n]);

            for (auto e : x) cout << e << " ";
        }

        cout << "\n";
        next:;
    }
}
