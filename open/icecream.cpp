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

        Matrix<T> C(r1, c2, numeric_limits<T>::lowest());
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] = max(C[i][j], A[i][k] + B[k][j]);

        return C;
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, a, b;
    cin >> n >> k >> a >> b;

    vector<double> t(k);
    for (auto &ti : t) cin >> ti;

    Matrix<double> u(k);
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            cin >> u[i][j];

            u[i][j] += t[i];
        }

    vector<Matrix<double>> bases{u};
    while (1LL << bases.size() <= n) bases.emplace_back(bases.back() * bases.back());

    auto mul = [&](Matrix<double> &A, const vector<double> &v, auto offset) -> vector<double> {
        auto u = v;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++) u[i] = max(u[i], A[i][j] + v[j] + offset);
        return u;
    };

    vector<double> value(k);
    auto pow = [&](int i, auto exponent, auto offset) {
        while (exponent) {
            if (exponent & 1) value = mul(bases[i], value, offset * (1LL << i));
            i++;
            exponent >>= 1;
        }
        return value;
    };

    double l = 0, r = 400, m;
    while (l + 1e-7 < r && l + l * 1e-7 < r) {
        m = l + (r - l) / 2;

        for (int i = 0; i < k; i++) value[i] = t[i] - m * (a + b);
        for (int i = 0; 1LL << (i + 1) <= n; i++) value = mul(bases[i], value, -m * a * (1LL << i));
        value = pow(0, n - bit_floor((unsigned) n), -m * a);

        if (*max_element(value.begin(), value.end()) > 0) l = m;
        else r = m;
    }
    cout << fixed << setprecision(6) << l;
}
