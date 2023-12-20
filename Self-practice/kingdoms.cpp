#include <bits/stdc++.h>
using namespace std;

void dfs(int v, vector<bool> &visited, vector<long long> &sum, vector<vector<long long>> &gold) {
    visited[v] = true;
    for (int i = 0; i < sum.size(); i++)
        if ((v & (1 << i)) && sum[i] < 0 && !visited[v - (1 << i)]) {
            for (int j = 0; j < sum.size(); j++)
                if (v & (1 << j)) sum[j] -= gold[i][j];

            dfs(v - (1 << i), visited, sum, gold);
            for (int j = 0; j < sum.size(); j++)
                if (v & (1 << j)) sum[j] += gold[i][j];
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        vector<vector<long long>> gold(n, vector<long long>(n));
        for (auto &row : gold)
            for (auto &num : row) cin >> num;

        vector<bool> visited(1 << n, false);
        vector<long long> sum(n, 0);
        for (auto &row : gold)
            for (int i = 0; i < n; i++) sum[i] += row[i];
        dfs((1 << n) - 1, visited, sum, gold);

        bool survivors = false;
        for (int i = 0; i < n; i++)
            if (visited[1 << i]) {
                cout << i + 1 << " ";
                survivors = true;
            }
        cout << (survivors ? "\n" : "0\n");
    }
}
