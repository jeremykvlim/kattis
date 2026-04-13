#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> pair<vector<long long>, vector<long long>> {
        vector<long long> pos1(1, 1e18), neg1(1, 1e18);
        (a[v] > 0 ? pos1 : neg1)[0] = a[v];
        for (int u : adj_list[v])
            if (u != prev) {
                auto [pos2, neg2] = self(self, u, v);
                vector<long long> temp_pos(pos1.size() + pos2.size(), 1e18), temp_neg(pos1.size() + pos2.size(), 1e18);
                for (int i = 0; i < pos1.size(); i++)
                    for (int j = 0; j < pos2.size(); j++) {
                        if (pos1[i] != 1e18 && pos2[j] != 1e18) temp_pos[i + j] = min(temp_pos[i + j], pos1[i] + pos2[j]);
                        if (pos1[i] != 1e18 && neg2[j] != 1e18) temp_neg[i + j] = min(temp_neg[i + j], pos1[i] + neg2[j]);
                        if (neg1[i] != 1e18 && pos2[j] != 1e18) temp_neg[i + j] = min(temp_neg[i + j], neg1[i] + pos2[j]);
                        if (neg1[i] != 1e18 && neg2[j] != 1e18) temp_neg[i + j] = min(temp_neg[i + j], neg1[i] + neg2[j]);
                        if (pos2[j] != 1e18 || neg2[j] < 0) {
                            temp_pos[i + j + 1] = min(temp_pos[i + j + 1], pos1[i]);
                            temp_neg[i + j + 1] = min(temp_neg[i + j + 1], neg1[i]);
                        }
                    }
                pos1 = temp_pos;
                neg1 = temp_neg;
            }
        return {pos1, neg1};
    };
    auto [pos, neg] = dfs(dfs);
    for (int i = 0; i < pos.size(); i++)
        if (pos[i] != 1e18 || neg[i] < 0) {
            cout << i;
            exit(0);
        }
}