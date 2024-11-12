#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(m);
    for (int &ai : a) {
        cin >> ai;

        ai = min(ai, n - ai);
    }

    int colleagues = 0;
    vector<int> count(n + 1, 0);
    unordered_set<int> visited{1 << (n - 1)};
    queue<pair<int, int>> q;
    q.emplace(1 << (n - 1), 0);
    while (!q.empty()) {
        auto [curr, day] = q.front();
        q.pop();

        vector<int> gaps;
        int c = 0;
        for (int i = 0, j = -1; i < n; i++)
            if ((curr >> i) & 1) {
                gaps.emplace_back(i - j);
                if (i - j == 1) c++;
                j = i;
            }
        colleagues = max(colleagues, c);

        auto dfs = [&](auto &&self, int pos = 0, int mask = 1) {
            if (pos == gaps.size()) {
                if ((mask >> a[day]) & 1) {
                    int next = 0, k = -1;
                    for (int i = 1; i <= n; i++)
                        for (int j = 0; j < count[i]; j++) {
                            k += i;
                            next |= (1 << k);
                        }

                    if (!visited.count(next)) {
                        q.emplace(next, day + 1);
                        visited.emplace(next);
                    }
                }
                return;
            }

            for (int i = 0; i <= gaps[pos] / 2; i++) {
                count[i]++;
                count[gaps[pos] - i]++;
                self(self, pos + 1, (mask << i) | (mask << (gaps[pos] - i)));
                count[i]--;
                count[gaps[pos] - i]--;
            }
        };

        if (day < m) dfs(dfs);
    }

    cout << colleagues;
}