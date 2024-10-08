#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, c;
    cin >> n >> r >> c;

    vector<vector<int>> camera(r, vector<int>(c)), brightness(r, vector<int>(c, 0));
    vector<vector<bool>> visited(r, vector<bool>(c, true));
    for (auto &row : camera)
        for (auto &pixel : row) cin >> hex >> pixel;

    if (n == 1) {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                if (camera[i][j]) visited[i][j] = false;
    } else {
        for (int i = n / 2; i < r - n / 2; i++)
            for (int j = n / 2; j < c - n / 2; j++) {
                int k = max(0, i - n), l = max(0, j - n);
                brightness[i][j] = brightness[i - 1][j] + brightness[i][j - 1] - brightness[i - 1][j - 1];
                if (camera[i - n / 2][j - n / 2] > (brightness[i][j] - brightness[k][j] - brightness[i][l] + brightness[k][l]) * 0xFFFF / (n * n)) visited[i][j] = false;
                if (!visited[i][j]) brightness[i][j]++;
            }
    }

    auto dfs = [&](auto &&self, int i, int j) -> void {
        visited[i][j] = true;
        vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
        for (int k = 0; k < 4; k++) {
            int R = i + dr[k], C = j + dc[k];
            if (0 <= R && R < r && 0 <= C && C < c && !visited[R][C]) self(self, R, C);
        }
    };
    int count = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (!visited[i][j]) {
                count++;
                dfs(dfs, i, j);
            }

    cout << count;
}
