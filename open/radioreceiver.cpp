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

    int c;
    cin >> c;

    for (int x = 1; x <= c; x++) {
        int n;
        cin >> n;

        vector<array<int, 2>> messages(n);
        for (auto &[t, p] : messages) cin >> p >> t;
        sort(messages.begin(), messages.end());

        int sum = messages[0][0] + messages[0][1], diff = messages[0][0] - messages[0][1];
        vector<vector<double>> A{{-1}};
        vector<double> b{0};
        for (int i = 1; i < n; i++) {
            auto [t, p] = messages[i];

            A.push_back({-2});
            b.emplace_back(t + p - sum);
            A.push_back({-2});
            b.emplace_back(t - p - diff);
            sum = max(sum, t + p);
            diff = max(diff, t - p);
        }
        auto [value, solution] = linear_program_solution(A, b, {-1});
        cout << "Case #" << x << ": " << fixed << setprecision(9) << solution[0] << "\n";
    }
}
