#include <bits/stdc++.h>
using namespace std;

void rref(vector<vector<double>> &matrix) {
    int r = matrix.size(), c = matrix[0].size();

    int pivot = 0;
    for (int i = 0; i < c - 1 && pivot < r; i++) {
        for (int j = pivot; j < r; j++)
            if (fabs(matrix[j][i]) > 1e-9) {
                matrix[pivot].swap(matrix[j]);
                break;
            }

        if (fabs(matrix[pivot][i]) < 1e-9) continue;
        auto temp = 1 / matrix[pivot][i];
        for (int j = pivot; j < c; j++) matrix[pivot][j] *= temp;

        for (int j = 0; j < r; j++)
            if (j != pivot && fabs(matrix[j][i]) > 1e-9) {
                temp = matrix[j][i] / matrix[pivot][i];
                for (int k = pivot; k < c; k++) matrix[j][k] -= matrix[pivot][k] * temp;
            }

        pivot++;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false)), meet(n, vector<bool>(n, false));
    vector<int> degree(n, 0);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_matrix[u][v] = adj_matrix[v][u] = true;
        degree[u]++;
        degree[v]++;
    }

    int s, t;
    cin >> s >> t;

    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) {
        meet[i][i] = true;
        q.emplace(i, i);
    }

    while (!q.empty()) {
        auto [u, v] = q.front();
        q.pop();

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (adj_matrix[u][i] && adj_matrix[v][j] && !meet[i][j]) {
                    meet[i][j] = true;
                    q.emplace(i, j);
                }
    }

    if (!meet[s][t]) {
        cout << "never meet";
        exit(0);
    }

    vector<vector<double>> matrix(n * n + n, vector<double>(n * n + n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j || !meet[i][j]) {
                matrix[i * n + j][i * n + j] = 1;
                continue;
            }

            matrix[i * n + j][i * n + j] = matrix[i * n + j][n * n] = degree[i] * degree[j];
            for (int k = 0; k < n; k++)
                for (int l = 0; l < n; l++)
                    if (adj_matrix[i][k] && adj_matrix[j][l]) matrix[i * n + j][k * n + l] = -1;
        }

    rref(matrix);
    cout << fixed << setprecision(6) << matrix[s * n + t][n * n];
}
