#include <bits/stdc++.h>
using namespace std;

vector<double> matvecmul(vector<vector<double>> &a, vector<double> &v) {
    int r = a.size(), c = v.size();

    vector<double> u(r, 0);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) u[i] += a[i][j] * v[j];

    return u;
}

vector<vector<double>> transpose(vector<vector<double>> &a) {
    int n = a.size(), m = a[0].size();
    vector<vector<double>> a_T(m, vector<double>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) a_T[j][i] = a[i][j];

    return a_T;
}

double norm(vector<double> &v, int n) {
    return sqrt(inner_product(v.begin(), v.begin() + n, v.begin(), 0.0));
}

vector<vector<double>> I(int size) {
    vector<vector<double>> I(size, vector<double>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

pair<vector<vector<double>>, vector<vector<double>>> QR_decomposition(vector<vector<double>> &a) {
    int n = a.size(), m = a[0].size();

    auto Q = I(n), R = a;
    vector<double> curr(n), v(n), u(max(n, m));
    for (int i = 0; i < min(n - 1, m); i++) {
        for (int j = 0; j < n - i; j++) curr[j] = R[j + i][i];
        fill(v.begin(), v.end(), 0);
        v[0] = norm(curr, n - i);
        for (int j = 0; j < n; j++) v[j] = curr[j] - v[j];
        auto temp = norm(v, n - i);
        if (fabs(temp) > 1e-9)
            for (auto &vi : v) vi /= temp;

        auto reflect = [&](vector<vector<double>> &P, bool left) {
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

    vector<vector<double>> a(d, vector<double>(n - 1));
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < d; j++) a[j][i] = 2 * results[i][j];

    auto [Q, R] = QR_decomposition(a);
    auto R_T = transpose(R);
    vector<double> b(n - 1, e[0]), y(d, 0);
    for (int i = 0; i < n - 1; i++) b[i] += pow(norm(results[i], d), 2) - e[i + 1];
    for (int i = 0; i < n - 1; i++) {
        if (fabs(R_T[i][i]) > 1e-9) y[i] = b[i] / R_T[i][i];
        for (int j = i + 1; j < n - 1; j++) b[j] -= R_T[j][i] * y[i];
    }
    y[d - 1] += sqrt(max(0.0, e[0] - pow(norm(y, d), 2)));

    auto x = matvecmul(Q, y);
    for (int i = 0; i < d; i++) cout << fixed << setprecision(5) << x[i] + base[i] << " ";
}
