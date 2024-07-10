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

    friend auto operator*(Matrix<T> &A, vector<T> &v) {
        int n = A.r, m = v.size();

        vector<T> u(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) u[i] += A[i][j] * v[j];

        return u;
    }

    friend auto operator*=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    Matrix<T> transpose() {
        Matrix<T> mat_T(c, r);

        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) mat_T[j][i] = mat[i][j];

        return mat_T;
    }
};

template <typename T>
T norm(vector<T> &v, int n) {
    return sqrt(inner_product(v.begin(), v.begin() + n, v.begin(), (T) 0));
}

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 1;

    return I;
}

template <typename T>
pair<Matrix<T>, Matrix<T>> QR_decomposition(Matrix<T> &A) {
    int n = A.r, m = A.c;

    auto Q = I<T>(n), R = A;
    vector<T> curr(n), v(n), u(max(n, m));
    for (int i = 0; i < min(n - 1, m); i++) {
        for (int j = 0; j < n - i; j++) curr[j] = R[j + i][i];
        
        fill(v.begin(), v.end(), 0);
        v[0] = norm(curr, n - i);
        for (int j = 0; j < n; j++) v[j] = curr[j] - v[j];
        
        T temp = norm(v, n - i);
        if (fabs(temp) > 1e-9)
            for (T &vi : v) vi /= temp;

        auto reflect = [&](Matrix<T> &P, bool left) {
            fill(u.begin(), u.end(), 0);
            for (int j = 0; j < (left ? n : m); j++)
                for (int k = i; k < n; k++)
                    u[j] += P[left ? j : k][left ? k : j] * v[k - i];

            for (int j = 0; j < (left ? n : m); j++)
                for (int k = i; k < n; k++)
                    P[left ? j : k][left ? k : j] -= 2 * u[j] * v[k - i];
        };
        reflect(Q, true);
        reflect(R, false);
    }

    return {Q, R};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d, n;
    cin >> d >> n;
    n = min(n, d + 1);

    vector<vector<double>> results(n - 1, vector<double>(d));
    vector<double> base(d), e(n);
    for (int i = 0; i < n; i++) {
        if (!i)
            for (auto &xi : base) cin >> xi;
        else
            for (int j = 0; j < d; j++) {
                cin >> results[i - 1][j];

                results[i - 1][j] -= base[j];
            }

        cin >> e[i];

        e[i] *= e[i];
    }

    if (n == 1) {
        base[d - 1] += sqrt(e[0]);
        for (auto xi : base) cout << fixed << setprecision(5) << xi << " ";
        exit(0);
    }

    Matrix<double> A(d, n - 1);
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < d; j++) A[j][i] = 2 * results[i][j];

    auto [Q, R] = QR_decomposition(A);
    auto R_T = R.transpose();
    vector<double> b(n - 1, e[0]), y(d, 0);
    for (int i = 0; i < n - 1; i++) b[i] += pow(norm(results[i], d), 2) - e[i + 1];

    auto forward_substitution = [&]() {
        for (int i = 0; i < n - 1; i++) {
            y[i] = b[i];
            for (int j = 0; j < i; j++) y[i] -= R_T[i][j] * y[j];
            if (fabs(R_T[i][i]) > 1e-9) y[i] /= R_T[i][i];
        }
    };
    forward_substitution();

    y[d - 1] += sqrt(max(0.0, e[0] - pow(norm(y, d), 2)));
    auto x = Q * y;
    for (int i = 0; i < d; i++) cout << fixed << setprecision(5) << x[i] + base[i] << " ";
}
