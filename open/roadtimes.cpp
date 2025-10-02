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

    int n;
    cin >> n;

    vector<vector<pair<int, int>>> adj_list(n);
    vector<vector<int>> indices(n, vector<int>(n, -1));
    vector<array<int, 3>> edges;
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int k;
            cin >> k;

            if (k > 0) {
                edges.push_back({i, j, k});
                adj_list[i].emplace_back(j, k);
                indices[i][j] = count++;
            }
        }

    vector<vector<double>> A;
    vector<double> b;
    for (auto [u, v, w] : edges) {
        vector<double> c(count, 0);
        c[indices[u][v]] = 1;

        A.emplace_back(c);
        b.emplace_back(w);
    }

    vector<int> dist(n, INT_MAX), prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    auto path = [&](int s, int d) -> tuple<vector<double>, vector<double>, int> {
        fill(dist.begin(), dist.end(), INT_MAX);
        fill(prev.begin(), prev.end(), -1);
        dist[s] = 0;
        pq.emplace(0, s);
        while (!pq.empty()) {
            auto [dv, v] = pq.top();
            pq.pop();

            if (dv != dist[v]) continue;

            for (auto [u, w] : adj_list[v])
                if (dist[u] > dv + w) {
                    dist[u] = dv + w;
                    pq.emplace(dv + w, u);
                    prev[u] = v;
                }
        }

        vector<int> p;
        while (d != s) {
            p.emplace_back(indices[prev[d]][d]);
            d = prev[d];
        }

        vector<double> c_pos(count, 0), c_neg(count, 0);
        int w = 0;
        for (int i : p) {
            c_pos[i] = 1;
            c_neg[i] = -1;
            w += edges[i][2];
        }

        return {c_pos, c_neg, w};
    };

    int r;
    cin >> r;

    while (r--) {
        int s, d, t;
        cin >> s >> d >> t;

        auto [c_pos, c_neg, w] = path(s, d);
        A.emplace_back(c_pos);
        A.emplace_back(c_neg);
        b.emplace_back(t - w);
        b.emplace_back(w - t);
    }

    int q;
    cin >> q;

    while (q--) {
        int s, d;
        cin >> s >> d;

        auto [c_pos, c_neg, w] = path(s, d);
        cout << fixed << setprecision(6) << s << " " << d << " " << -linear_program_solution(A, b, c_neg).first + w << " " << linear_program_solution(A, b, c_pos).first + w << "\n";
    }
}