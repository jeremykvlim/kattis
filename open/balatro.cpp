#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<bool> type(n);
    vector<int> v(n), indices;
    int max_v = 0;
    for (int i = 0; i < n; i++) {
        char c;
        cin >> c >> v[i];

        max_v = max(max_v, v[i]);
        type[i] = c == 'm';
        if (type[i]) indices.emplace_back(i);
    }

    int size = indices.size();
    vector<vector<int>> count(size + 1, vector<int>(max_v + 1, 0));
    for (int i = 0, j = 0; i < n; i++) {
        if (type[i]) j++;
        else count[j][v[i]]++;
    }

    vector<int> adj_list(size, 0);
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            if (v[indices[i]] <= v[indices[j]]) adj_list[i] |= 1 << j;

    vector<pair<long long, int>> a;
    vector<long long> score(n + 1, 0);
    auto dfs = [&](auto &&self, int i, int mask = 0, int depth = 0) -> void {
        if (!~i) {
            a.clear();
            int product = 1;
            for (int j = size; ~j; j--) {
                if ((mask >> j) & 1) product *= v[indices[j]];

                for (int vi = 2; vi <= max_v; vi++)
                    if (count[j][vi]) a.emplace_back((long long) vi * product, count[j][vi]);
            }
            sort(a.rbegin(), a.rend());

            auto sum = 0LL;
            int d = depth + 1;
            for (auto [vi, c] : a)
                for (int j = 0; j < c; j++, d++) {
                    sum += vi;
                    if (d <= n) score[d] = max(score[d], sum);
                }
            return;
        }

        self(self, i - 1, mask, depth);
        if ((mask | adj_list[i]) == mask && depth < k) self(self, i - 1, mask | (1 << i), depth + 1);
    };
    dfs(dfs, size - 1);

    for (int i = 1; i <= n; i++) {
        score[i] = max(score[i], score[i - 1]);

        cout << score[i] << "\n";
    }
}
