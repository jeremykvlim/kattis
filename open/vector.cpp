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

    void add_column(const vector<T> &column) {
        if (!r) mat.resize(r = column.size());
        for (int i = 0; i < r; i++) mat[i].emplace_back(column[i]);
        c++;
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 1;
    return I;
}

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

template <typename T>
vector<complex<T>> quadratic_roots(T a, T b, T c) {
    if (fabs(a) < 1e-8 && fabs(b) < 1e-8) return {};
    if (fabs(a) < 1e-8) return {(-c / b)};

    complex<T> discriminant(b * b - 4 * a * c, 0);
    return {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, n;
    cin >> d >> n;

    vector<double> base(d);
    for (auto &xi : base) cin >> xi;

    double e0;
    cin >> e0;

    if (n == 1) {
        base[d - 1] += e0;
        for (auto xi : base) cout << fixed << setprecision(5) << xi << " ";
        exit(0);
    }

    n = min(n - 1, d);
    e0 *= e0;
    Matrix<double> A(n, d + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            double xi;
            cin >> xi;

            A[i][j] = xi - base[j];
        }

        double ei;
        cin >> ei;

        A[i][d] = (inner_product(A[i].begin(), A[i].begin() + d, A[i].begin(), 0.) + e0 - ei * ei) / 2;
    }
    rref(A);

    if (n == d) {
        for (int i = 0; i < d; i++) cout << fixed << setprecision(5) << base[i] + A[i][d] << " ";
        exit(0);
    }

    int m = d - n;
    auto B = I<double>(m);
    B.add_column(vector<double>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            B[j][m] += A[i][j + n] * A[i][d];
            for (int k = 0; k < m; k++) B[j][k] += A[i][j + n] * A[i][k + n];
        }
    rref(B);

    vector<double> x(d, 0), dir(d, 0);
    for (int i = 0; i < n; i++) {
        x[i] = A[i][d];
        for (int j = 0; j < m; j++) x[i] -= A[i][j + n] * B[j][m];
        dir[i] = -A[i][n];
    }
    for (int j = 0; j < m; j++) x[j + n] = B[j][m];
    dir[n] = 1;

    auto r = quadratic_roots(inner_product(dir.begin(), dir.end(), dir.begin(), 0.),
                             2 * inner_product(x.begin(), x.end(), dir.begin(), 0.),
                             inner_product(x.begin(), x.end(), x.begin(), 0.) - e0)[0].real();
    for (int i = 0; i < d; i++) x[i] += base[i] + r * dir[i];
    for (auto xi : x) cout << fixed << setprecision(5) << xi << " ";
}
