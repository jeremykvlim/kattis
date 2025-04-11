#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int k, n, m, r, c;
    cin >> k >> n >> m >> s >> r >> c;

    if (k & 1) {
        cout << "impossible";
        exit(0);
    }

    int white = 0;
    for (char ch : s) white += ch == 'W';

    string path, temp;
    vector<char> dir{'S', 'E', 'N', 'W'};
    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    vector<vector<bool>> visited(n + 2, vector<bool>(m + 2, false));
    for (int i = 0; i <= n + 1; i++)
        for (int j = 0; j <= m + 1; j++) visited[i][j] = !(1 <= i && i <= n && 1 <= j && j <= m);

    for (int d1 = 0; d1 < 4; d1++) {
        int d0 = d1;
        bool found = false;
        auto backtrack = [&](auto &&self, int d1, int x, int y, int i = 0) {
            if (i == k) {
                if (x != r || y != c || d1 != d0 || white != 0) return;
                found = true;
                if (path.empty() || path > temp) path = temp;
                return;
            }

            if (visited[x][y]) return;
            visited[x][y] = true;
            for (int d2 : {1, 2, 0, 3}) {
                if ((d1 ^ d2) == 2 || (d1 == d2 && s[i] == 'B') || (d1 != d2 && (s[i] == 'W' || s[i] == '-' || s[(i + 1) % k] == 'B' || s[(i + k - 1) % k] == 'B'))) continue;

                bool undo1 = false, undo2 = false;
                if (d1 != d2 && s[(i + 1) % k] == 'W') {
                    undo1 = true;
                    s[(i + 1) % k] = '-';
                    white--;
                }
                if (d1 != d2 && s[(i + k - 1) % k] == 'W') {
                    undo2 = true;
                    s[(i + k - 1) % k] = '-';
                    white--;
                }

                temp += dir[d2];
                if (abs(x + dx[d2] - r) + abs(y + dy[d2] - c) <= k - (i + 1) && !found) self(self, d2, x + dx[d2], y + dy[d2], i + 1);
                temp.pop_back();

                if (undo2) {
                    s[(i + k - 1) % k] = 'W';
                    white++;
                }
                if (undo1) {
                    s[(i + 1) % k] = 'W';
                    white++;
                }
            }
            visited[x][y] = false;
        };
        backtrack(backtrack, d1, r, c);
    }

    if (path.empty()) cout << "impossible";
    else cout << path;
}
