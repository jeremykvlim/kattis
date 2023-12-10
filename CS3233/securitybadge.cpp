#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, B, s, d;
    cin >> n >> l >> B >> s >> d;

    vector<vector<tuple<int, int, int>>> locks(n + 1);
    vector<int> badges;

    for (int i = 0; i < l; i++) {
        int a, b, x, y;
        cin >> a >> b >> x >> y;
        locks[a].emplace_back(b, x, y);
        badges.emplace_back(x);
        badges.emplace_back(y);
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

            for (auto &[b, x, y] : locks[v])
                if (badges[i - 1] >= x && badges[i] <= y && !visited[b]) {
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
