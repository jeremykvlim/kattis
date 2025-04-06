#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, y;
    cin >> x >> y;

    vector<vector<int>> grid(y, vector<int>(x));
    for (auto &row : grid)
        for (auto &hij : row) cin >> hij;

    int count = 0;
    vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
    vector<vector<bool>> visited(y, vector<bool>(x, false));
    for (int i = 0; i < y; i++)
        for (int j = 0; j < x; j++)
            if (!visited[i][j]) {
                bool lower = false;
                int size = 0;
                visited[i][j] = true;
                queue<pair<int, int>> q;
                q.emplace(i, j);
                while (!q.empty()) {
                    auto [R, C] = q.front();
                    q.pop();

                    size++;
                    for (int k = 0; k < 4; k++) {
                        int r = R + dr[k], c = C + dc[k];
                        if (0 <= r && r < y && 0 <= c && c < x) {
                            lower |= grid[r][c] < grid[i][j];
                            if (grid[r][c] == grid[i][j] && !visited[r][c]){
                                visited[r][c] = true;
                                q.emplace(r, c);
                            }
                        }
                    }
                }
                if (!lower) count += size;
            }
    cout << count;
}
