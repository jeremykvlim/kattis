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

    int T;
    cin >> T;

    while (T--) {
        int xs, ys, xt, yt, n;
        cin >> xs >> ys >> xt >> yt >> n;

        int dx1 = xt - xs;
        vector<vector<double>> A{{-1}};
        vector<double> b{0};
        while (n--) {
            int x, y;
            cin >> x >> y;

            int dx2 = x - xs;
            int delta = y * dx1 - ys * (dx1 - dx2) - yt * dx2;
            if (delta > 0) {
                A.push_back({-1});
                b.emplace_back(-(double) delta / (dx2 * (dx1 - dx2)));
            }
        }
        auto [_, solution] = linear_program_solution(A, b, {-1});

        double altitude = max(ys, yt), coeff = max(0., solution[0]);
        if (coeff > 0) {
            auto t = clamp(0.5 + (yt - ys) / (2 * coeff * dx1), 0., 1.);
            altitude = ys * (1 - t) + yt * t + coeff * dx1 * t * (1 - t);
        }
        cout << fixed << setprecision(5) << altitude << "\n";
    }
}
