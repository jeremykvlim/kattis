#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, vector<T>> linear_program_solution(const vector<vector<T>> &A, const vector<T> &b, const vector<T> &c) {
    int m = b.size(), n = c.size();

    vector<vector<T>> tableau(m + 2, vector<T>(n + 2));
    vector<T> basic(m), non_basic(n + 1);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) tableau[i][j] = A[i][j];
        basic[i] = n + i;
        tableau[i][n] = -1;
        tableau[i][n + 1] = b[i];
    }

    iota(non_basic.begin(), non_basic.end(), 0);
    for (int j = 0; j < n; j++) tableau[m][j] = -c[j];
    non_basic[n] = -1;
    tableau[m + 1][n] = 1;

    auto pivot = [&](int row, int col) {
        T inv = 1 / tableau[row][col];
        for (int i = 0; i < m + 2; i++)
            if (i != row && fabs(tableau[i][col]) > 1e-8) {
                T temp = tableau[i][col] * inv;
                for (int j = 0; j < n + 2; j++) tableau[i][j] -= tableau[row][j] * temp;
                tableau[i][col] = tableau[row][col] * temp;
            }
        for (int i = 0; i < m + 2; i++)
            if (i != row) tableau[i][col] *= -inv;
        for (int j = 0; j < n + 2; j++)
            if (j != col) tableau[row][j] *= inv;
        tableau[row][col] = inv;
        swap(basic[row], non_basic[col]);
    };

    auto simplex = [&](int phase) -> bool {
        for (int r = m + (phase == 1);;) {
            int col = -1;
            for (int j = 0; j <= n; j++) {
                if (phase == 2 && non_basic[j] == -1) continue;
                if (col == -1 || pair<T, T>(tableau[r][j], non_basic[j]) < pair<T, T>(tableau[r][col], non_basic[col])) col = j;
            }
            if (tableau[r][col] >= -1e-8) return true;

            int row = -1;
            for (int i = 0; i < m; i++) {
                if (tableau[i][col] <= 1e-8) continue;
                if (row == -1 || pair<T, T>(tableau[i][n + 1] / tableau[i][col], basic[i]) < pair(tableau[row][n + 1] / tableau[row][col], basic[row])) row = i;
            }
            if (row == -1) return false;

            pivot(row, col);
        }
    };

    int row = 0;
    for (int i = 1; i < m; i++)
        if (tableau[i].back() < tableau[row].back()) row = i;

    if (tableau[row][n + 1] <= -1e-8) {
        pivot(row, n);
        if (!simplex(1) || tableau[m + 1][n + 1] < -1e-8) return {-numeric_limits<T>::infinity(), {}};
        for (int i = 0; i < m; i++)
            if (basic[i] == -1) {
                int col = -1;
                for (int j = 0; j <= n; j++)
                    if (col == -1 || pair<T, T>{tableau[i][j], non_basic[j]} < pair<T, T>{tableau[i][col], non_basic[col]}) col = j;
                pivot(i, col);
            }
    }
    if (!simplex(2)) return {numeric_limits<T>::infinity(), {}};

    vector<T> solution(n, 0);
    for (int i = 0; i < m; i++)
        if (basic[i] < n) solution[basic[i]] = tableau[i][n + 1];
    return {tableau[m][n + 1], solution};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s, R, d;
    cin >> s >> R >> d;

    vector<vector<vector<pair<int, double>>>> stations(s + 1);
    while (d--) {
        int i, n;
        cin >> i >> n;

        stations[i].emplace_back(vector<pair<int, double>>(n));
        for (auto &[o, p] : stations[i].back()) {
            cin >> o >> p;
            p *= 1e-2;
        }
    }

    vector<vector<double>> memo(s + R + 1, vector<double>(R, 0));
    auto dp = [&](const vector<double> &w) {
        for (int k = 0; k < R; k++) {
            fill(memo[s + k + 1].begin(), memo[s + k + 1].end(), 0);
            memo[s + k + 1][k] = 1;
        }

        for (int i = s; i; i--) {
            double highest = 0;
            for (const auto &output : stations[i]) {
                vector<double> temp(R, 0);
                for (auto [o, p] : output) transform(memo[o].begin(), memo[o].end(), temp.begin(), temp.begin(), [&](auto x, auto y) { return p * x + y; });

                auto dot = inner_product(w.begin(), w.end(), temp.begin(), 0.);
                if (highest < dot) {
                    highest = dot;
                    memo[i] = temp;
                }
            }
        }
        return inner_product(w.begin(), w.end(), memo[1].begin(), 0.);
    };

    double l = 0, r = dp(vector<double>(R, 1. / R));
    vector<vector<double>> planes{memo[1]};
    while (l + 1e-8 < r && l + l * 1e-8 < r) {
        int p = planes.size(), m = p + 2, n = R + 1;
        vector<vector<double>> A(m, vector<double>(n, 0));
        vector<double> b(m, 0), c(n, 0);
        for (int k = 0; k < p; k++) {
            for (int i = 0; i < R; i++) A[k][i] = planes[k][i];
            A[k][R] = -1;
        }
        for (int i = 0; i < R; i++) {
            A[p][i] = 1;
            A[p + 1][i] = -1;
        }
        b[p] = 1;
        b[p + 1] = c[R] = -1;
        auto [_, solution] = linear_program_solution(A, b, c);

        double sum = 0;
        vector<double> w(R);
        for (int i = 0; i < R; i++) sum += w[i] = max(0., solution[i]);
        for (auto &wi : w) wi /= sum;
        l = max(l, solution[R]);
        r = min(r, dp(w));
        planes.emplace_back(memo[1]);
    }
    cout << fixed << setprecision(6) << r * 1e2;
}
