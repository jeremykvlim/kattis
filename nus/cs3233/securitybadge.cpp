#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, B, s, d;
    cin >> n >> l >> B >> s >> d;

    vector<vector<array<int, 3>>> locks(n + 1);
    vector<int> badges(2 * l);
    for (int i = 0; i < l; i++) {
        int a, b, x, y;
        cin >> a >> b >> x >> y;

        locks[a].push_back({b, x, y});
        badges[2 * i] = x;
        badges[2 * i + 1] = y;
    }
    sort(badges.begin(), badges.end());

    int count = 0;
    bool prev = false, pass;
    for (int i = 1; i < 2 * l; i++) {
        vector<bool> visited(n, false);
        pass = false;

        queue<int> q;
        q.emplace(s);
        visited[s] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (v == d) {
                pass = true;
                break;
            }

            for (auto [b, x, y] : locks[v])
                if (!visited[b] && badges[i - 1] >= x && badges[i] <= y) {
                    q.emplace(b);
                    visited[b] = true;
                }
        }

        if (pass) count += badges[i] - badges[i - 1] + 1;
        if (prev && pass) count--;
        prev = pass;
    }

    cout << count;
}
