#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, vector<T>> linear_program_solution(const vector<vector<T>> &A, const vector<T> &b, const vector<T> &c) {
    int m = b.size(), n = c.size();

    vector<vector<T>> tableau(m + 2, vector<T>(n + 2));
    vector<int> basic(m), non_basic(n + 1);
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
                if (col == -1 || make_pair(tableau[r][j], non_basic[j]) < make_pair(tableau[r][col], non_basic[col])) col = j;
            }
            if (tableau[r][col] >= -1e-8) return true;

            int row = -1;
            for (int i = 0; i < m; i++) {
                if (tableau[i][col] <= 1e-8) continue;
                if (row == -1 || make_pair(tableau[i][n + 1] / tableau[i][col], basic[i]) < make_pair(tableau[row][n + 1] / tableau[row][col], basic[row])) row = i;
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
                    if (col == -1 || make_pair(tableau[i][j], non_basic[j]) < make_pair(tableau[i][col], non_basic[col])) col = j;
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

    for (;;) {
        array<array<double, 2>, 4> p;
        for (auto &[x, y] : p) {
            int z;
            cin >> x >> y >> z;

            if (!x && !y && !z) exit(0);
        }

        auto scale = 1e4;
        vector<vector<double>> A{{scale, scale, scale, 0},
                                 {-scale, -scale, -scale, 0},
                                 {-scale, 0, 0, 1},
                                 {0, -scale, 0, 1},
                                 {0, 0, -scale, 1},
                                 {p[0][0], p[1][0], p[2][0], 0},
                                 {-p[0][0], -p[1][0], -p[2][0], 0},
                                 {p[0][1], p[1][1], p[2][1], 0},
                                 {-p[0][1], -p[1][1], -p[2][1], 0}};
        auto value = linear_program_solution(A, vector<double>{scale, -scale, 0, 0, 0, p[3][0], -p[3][0], p[3][1], -p[3][1]}, vector<double>{0, 0, 0, 1}).first;
        cout << (isfinite(value) && value > 1e-7 ? "YES\n" : "NO\n");
    }
}
