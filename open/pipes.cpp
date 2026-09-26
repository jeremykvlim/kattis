#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    while (k--) {
        int m, n, l;
        cin >> m >> n >> l;

        vector<int> connect(2 * (m + n), -1);
        while (l--) {
            int s, t;
            cin >> s >> t;

            connect[s] = t;
            connect[t] = s;
        }


        vector<int> dr{0, 1, 0, -1}, dc{-1, 0, 1, 0};
        vector<bool> visited(m * n, false);
        auto dfs = [&](auto &&self, int v, int in, int t, int out) -> bool {
            visited[v] = true;
            if (v == t && in != out) return true;

            int r = v / n, c = v % n;
            for (int k = 1; k < 4; k++) {
                int d = (in + k) & 3, x = r + dr[d], y = c + dc[d];
                if (0 <= x && x < m && 0 <= y && y < n) {
                    int u = x * n + y;
                    if (!visited[u] && self(self, u, d ^ 2, t, out)) return true;
                }
            }
            return false;
        };

        stack<int> st;
        bool possible = true;
        for (int i = 0; i < 2 * (m + n) && possible; i++)
            if (~connect[i]) {
                if (i < connect[i]) st.emplace(i);
                else {
                    if (st.empty() || st.top() != connect[i]) {
                        possible = false;
                        break;
                    }

                    int j = st.top();
                    st.pop();

                    auto pos = [&](int v) -> pair<int, int> {
                        if (v < m) return {v * n, 0};
                        v -= m;
                        if (v < n) return {(m - 1) * n + v, 1};
                        v -= n;
                        if (v < m) return {(m - 1 - v) * n + n - 1, 2};
                        v -= m;
                        return {n - 1 - v, 3};
                    };
                    auto [s, a] = pos(j);
                    auto [t, b] = pos(i);
                    if (!visited[s] || !visited[t] || !dfs(dfs, s, a, t, b)) possible = false;
                }
            }

        cout << (possible ? "YES\n" : "NO\n");
    }
}
