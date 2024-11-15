#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false));
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_matrix[a - 1][b - 1] = adj_matrix[b - 1][a - 1] = true;
    }

    vector<int> count(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (adj_matrix[i][j]) count[i]--;

    vector<bool> s(n, false);
    for (int i = min_element(count.begin(), count.end()) - count.begin(); count[i] < 0; i = min_element(count.begin(), count.end()) - count.begin()) {
        s[i] = !s[i];
        count[i] = 0;
        for (int j = 0; j < n; j++) {
            if (adj_matrix[i][j]) {
                auto c = [&]() {
                    return s[i] != s[j] ? 1 : -1;
                };
                count[i] += c();
                count[j] += 2 * c();
            }
        }
    }
    for (bool b : s) cout << (b ? "S" : "P");
}
