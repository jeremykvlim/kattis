#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> p3_table(38, 1);
    for (int i = 1; i < 38; i++) p3_table[i] = 3 * p3_table[i - 1];

    int n;
    vector<vector<int>> count(38, vector<int>(60)), prev(38, vector<int>(60));
    vector<int> p2_l(38), p2_r(38), starts(39);
    while (cin >> n && n) {
        bool possible = true;
        int max_p3 = 0;
        for (auto &row : count)
            fill(row.begin(), row.end(), 0);
        fill(p2_l.begin(), p2_l.end(), 60);
        fill(p2_r.begin(), p2_r.end(), -1);
        for (int i = 0; i < n; i++) {
            long long x;
            cin >> x;

            int p2 = countr_zero((unsigned long long) x);
            x >>= p2;

            int p3 = lower_bound(p3_table.begin(), p3_table.end(), x) - p3_table.begin();
            if (p3 == 38 || p3_table[p3] != x) {
                possible = false;
                continue;
            }

            count[p3][p2]++;
            p2_l[p3] = min(p2_l[p3], p2);
            p2_r[p3] = max(p2_r[p3], p2);
            max_p3 = max(max_p3, p3);
        }

        if (!possible) {
            cout << "NO\n";
            continue;
        }

        auto dp = 1LL;
        for (int p3 = 0; p3 <= max_p3 && dp; p3++) {
            fill(prev[p3].begin(), prev[p3].end(), -1);
            auto temp = 0LL;
            for (int p2 = 0; p2 < 60; p2++)
                if ((dp >> p2) & 1 && count[p3][p2])
                    for (int i = p2_l[p3]; i <= p2_r[p3]; i++)
                        if (count[p3][i] && !~prev[p3][i]) {
                            int j = p2_l[p3], e = 0;
                            for (; j <= p2_r[p3]; j++) {
                                e = 2 * count[p3][j] - (j == p2) - (j == i) - e;
                                if (e < (j < p2_r[p3])) break;
                            }

                            if (j > p2_r[p3] && !e) {
                                prev[p3][i] = p2;
                                temp |= 1LL << i;
                            }
                        }

            dp = temp;
        }

        if (!dp) {
            cout << "NO\n";
            continue;
        }

        starts[max_p3 + 1] = countr_zero((unsigned long long) dp);
        for (int p3 = max_p3; ~p3; p3--) starts[p3] = prev[p3][starts[p3 + 1]];

        cout << "YES\n";
        for (int p3 = 0; p3 <= max_p3; p3++) {
            vector<int> edge(60, 0);
            for (int p2 = p2_l[p3], e = 0; p2 < p2_r[p3]; p2++) edge[p2] = e = 2 * count[p3][p2] - (p2 == starts[p3]) - (p2 == starts[p3 + 1]) - e;

            vector<int> path;
            stack<int> s;
            s.emplace(starts[p3]);
            while (!s.empty()) {
                int p2 = s.top();

                if (p2 > p2_l[p3] && edge[p2 - 1]) {
                    edge[p2 - 1]--;
                    s.emplace(p2 - 1);
                } else if (p2 < p2_r[p3] && edge[p2]) {
                    edge[p2]--;
                    s.emplace(p2 + 1);
                } else {
                    path.emplace_back(p2);
                    s.pop();
                }
            }

            reverse(path.begin(), path.end());
            for (int p2 : path) cout << p3_table[p3] * (1LL << p2) << " ";
        }
        cout << "\n";
    }
}
