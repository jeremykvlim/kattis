#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;

        vector<int> p(n + 1), r(n + 1), d(n + 1);
        vector<vector<int>> adj_list(3e4 + 1);
        vector<bool> visited(3e4 + 1, false);
        for (int i = 1; i <= n; i++) {
            cin >> p[i] >> r[i] >> d[i];

            adj_list[r[i]].emplace_back(i);
            visited[r[i]] = visited[d[i]] = true;
        }

        vector<int> t, print;
        vector<unordered_map<int, int>> count(3e4 + 1);
        vector<vector<array<int, 3>>> files(n);
        for (int i = 1, j = 0; i <= 3e4; i++) {
            if (visited[i]) {
                j = i;
                t.emplace_back(i);
            }

            vector<int> temp;
            for (int k : print) {
                if (p[k]) {
                    if (d[k] <= i) {
                        cout << "NO\n";
                        goto next;
                    }

                    temp.emplace_back(k);
                }
            }
            print = temp;

            for (int k : adj_list[i]) print.emplace_back(k);
            sort(print.begin(), print.end(), [&](int i, int j) {return d[i] - p[i] < d[j] - p[j];});

            for (int k = 0; k < min(m, (int) print.size()); k++) {
                p[print[k]]--;
                count[j][print[k]]++;
            }
        }

        cout << "YES\n";
        for (int i = 1; i < t.size(); i++) {
            int x = t[i - 1], y, z = 1;
            for (auto [j, freq] : count[t[i - 1]]) {
                auto add = [&]() {
                    y = min(freq, t[i] - x) + x;
                    files[j - 1].push_back({x, y, z});
                    freq -= y - x;
                    x = y;

                    if (x == t[i]) {
                        x = t[i - 1];
                        z++;
                    }
                };
                add();
                if (!freq) continue;
                add();
            }
        }

        for (auto f : files) {
            cout << f.size() << "\n";
            for (auto [x, y, z] : f) cout << x << " " << y << " " << z << "\n";
        }
        cout << "\n";
        next:;
    }
}
