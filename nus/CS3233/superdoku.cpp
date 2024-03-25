#include <bits/stdc++.h>
using namespace std;

bool dfs(int i, int j, vector<vector<int>> &grid, vector<bool> &visited, vector<int> &matched, vector<vector<bool>> &col) {
    for (int v = 1; v < grid.size(); v++)
        if (!visited[v] && !col[j][v]) {
            visited[v] = true;
            if (!matched[v] || dfs(i, matched[v], grid, visited, matched, col)) {
                col[matched[v]][v] = false;
                col[j][v] = 1;
                grid[i][v] = matched[v] = j;
                return true;
            }
        }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> grid(n + 1, vector<int>(n + 1));
    vector<vector<bool>> row(n + 1, vector<bool>(n + 1)), col(n + 1, vector<bool>(n + 1));

    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= n; j++) {
            cin >> grid[i][j];
            if (row[grid[i][j]][i] || col[grid[i][j]][j]) {
                cout << "no\n";
                exit(0);
            }
            row[grid[i][j]][i] = col[grid[i][j]][j] = true;
        }


    for (int i = k + 1; i <= n; i++) {
        vector<int> matched(n + 1);
        for (int j = 1; j <= n; j++) {
            vector<bool> visited(n + 1, false);
            if (!dfs(i, j, grid, visited, matched, col)) {
                cout << "no\n";
                exit(0);
            }
        }
    }

    cout << "yes\n";
    for (int i = 1; i <= n; i++) {
        cout << grid[i][1];
        for (int j = 2; j <= n; j++) cout << " " << grid[i][j];
        cout << "\n";
    }
}
