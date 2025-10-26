#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, p;
    cin >> n >> m >> p;

    vector<string> name(n);
    vector<vector<tuple<deque<pair<int, int>>, int, int>>> buckets(n, vector<tuple<deque<pair<int, int>>, int, int>>(p));
    for (int i = 0; i < n; i++) {
        cin >> name[i];

        vector<vector<int>> quo(p);
        for (int _ = 0; _ < m; _++) {
            int x;
            cin >> x;

            quo[x % p].emplace_back(x / p);
        }

        for (int r = 0; r < p; r++) {
            sort(quo[r].begin(), quo[r].end());
            auto &[dq, q0, len] = buckets[i][r];
            q0 = -1;
            for (int j = 0, k = 1; j < quo[r].size(); j = k) {
                int q = quo[r][j];
                for (; k < quo[r].size() && quo[r][k] == q; k++);
                if (q0 == -1) {
                    q0 = q;
                    len = k - j;
                } else dq.emplace_back(q, k - j);
            }
        }
    }

    vector<int> residues(n, -1), vals(n, 1e9);
    auto compute = [&](int i) {
        residues[i] = -1;
        vals[i] = 1e9;
        for (int r = 0; r < p; r++) {
            auto &[dq, q, len] = buckets[i][r];
            if (q != -1) {
                int v = r + p * q;
                if (vals[i] > v) {
                    vals[i] = v;
                    residues[i] = r;
                }
            }
        }
    };
    for (int i = 0; i < n; i++) compute(i);

    for (auto alive = (1LL << n) - 1; alive;) {
        int out = -1;
        for (int i = 0; i < n; i++) {
            if (!((alive >> i) & 1)) continue;
            if (out == -1 || vals[i] < vals[out] || (vals[i] == vals[out] && name[i] < name[out])) out = i;
        }

        auto &[dq1, q1, len1] = buckets[out][residues[out]];
        if (q1 != -1) {
            if (!--len1) {
                if (!dq1.empty()) {
                    tie(q1, len1) = dq1.front();
                    dq1.pop_front();
                } else q1 = -1;
            }
        }

        compute(out);
        if (residues[out] == -1) {
            alive &= ~(1LL << out);
            cout << name[out] << " ";
        }

        for (int i = 0; i < n; i++)
            if ((alive >> i) & 1 && i != out) {
                auto &[dq2, q2, len2] = buckets[i][residues[i]];
                if (q2 == -1) continue;

                if (!--len2) {
                    if (!dq2.empty() && dq2.front().first == q2 + 1) {
                        auto [q, l] = dq2.front();
                        dq2.pop_front();

                        q2++;
                        len2 = l + 1;
                    } else {
                        q2++;
                        len2 = 1;
                    }
                    compute(i);
                } else {
                    if (!dq2.empty() && dq2.front().first == q2 + 1) dq2.front().second++;
                    else dq2.emplace_front(q2 + 1, 1);
                }
            }
    }
}
