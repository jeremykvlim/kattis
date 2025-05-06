#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> pos;
    for (int i = 0; i < n; i++)
        for (int x = i + 1, y = i % 5 + 1; y <= n; y += 5) {
            cout << x << " " << y << "\n" << flush;

            string s;
            cin >> s;

            if (s == "hit") pos.emplace_back(x, y);
        }

    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    for (int i = 0; i < pos.size(); i++)
        for (int j = 1; j <= 4; j++)
            for (int k = 0; k < 4; k++) {
                int x = pos[i].first + dx[k] * j, y = pos[i].second + dy[k] * j;
                if (0 < x && x <= n && 0 < y && y <= n) {
                    cout << x << " " << y << "\n" << flush;

                    string s;
                    cin >> s;

                    if (s == "sunk")
                        if (!--m) exit(0);
                }
            }
}
