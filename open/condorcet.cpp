#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> delta(n, vector<int>(n, 0));
    while (m--) {
        string s;
        int k;
        cin >> s >> k;

        for (int i = 0; i < s.size(); i++)
            for (int j = i + 1; j < s.size(); j++) {
                int u = s[i] - 'A', v = s[j] - 'A';
                delta[u][v] += k;
                delta[v][u] -= k;
            }
    }

    int voters = INT_MAX;
    for (int b = 0; b < pow(n, n); b++) {
        vector<int> lost_to(n);
        for (int i = 0, temp = b; i < n; i++) {
            lost_to[i] = temp % n;
            temp /= n;
        }
        if (ranges::any_of(views::iota(0, n), [&](int i) { return lost_to[lost_to[i]] == i; })) continue;

        int v = 0;
        for (int i = 0; i < n; i++) {
            vector<int> cycle;
            for (int _ = 0, j = lost_to[i]; _ <= n; _++, j = lost_to[j]) {
                cycle.emplace_back(j);
                if (j == i) break;
            }

            if (cycle.size() > n) v = max(v, delta[i][lost_to[i]] + 1);
            else {
                vector<int> votes;
                for (int j : cycle) votes.emplace_back(delta[j][lost_to[j]]);
                sort(votes.begin(), votes.end());

                int l = -1, r = 1e9, mid;
                while (l + 1 < r) {
                    mid = l + (r - l) / 2;

                    int extra = 0;
                    for (int d : votes)
                        if (mid > d) extra += (mid - d) / 2;
                        else {
                            extra = -1;
                            break;
                        }

                    if (extra < mid) l = mid;
                    else r = mid;
                }
                v = max(v, r);
            }
        }
        voters = min(voters, v);
    }
    cout << voters;
}
