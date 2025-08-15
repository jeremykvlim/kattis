#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    string s;
    cin >> n >> k >> m >> s;

    if (n == 1) {
        cout << 1;
        exit(0);
    }

    vector<vector<int>> indices(26);
    for (int i = 0; i < m; i++) indices[s[i] - 'a'].emplace_back(i + 1);

    vector<int> next(n, m + 1);
    for (int c = 0; c < n; c++)
        if (!indices[c].empty()) next[c] = indices[c][0];

    auto compute = [&]() -> array<int, 3> {
        int latest = -1, ch = -1, second_latest = -1;
        for (int c = 0; c < n; c++) {
            int i = next[c];
            if (latest < i) {
                second_latest = latest;
                latest = i;
                ch = c;
            } else second_latest = max(second_latest, i);
        }
        return {latest, second_latest, ch};
    };
    int count = compute()[0];

    if (count > m) {
        cout << -1;
        exit(0);
    }

    vector<int> path(m + 1, m + 1), r(n, 0);
    for (int i = 1; i <= m; i++) {
        int c = s[i - 1] - 'a';
        if (r[c] < indices[c].size() && next[c] == i) {
            r[c]++;
            next[c] = r[c] < indices[c].size() ? indices[c][r[c]] : m + 1;
        }
        auto [latest, second_latest, ch] = compute();
        path[i] = (c != ch ? latest : second_latest);
    }

    while (k--) {
        count = path[count];
        if (count > m) {
            cout << -1;
            exit(0);
        }
    }
    cout << count;
}
