#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int hit = 0, miss = 0;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        for (int j = 0; j < n; j++) {
            int mask = 1 << (i * n + j);
            
            if (s[j] == 'O') hit += mask;
            else if (s[j] == 'X') miss += mask;
        }
    }

    vector<vector<int>> adj_list(k);
    for (int K = 0; K < k; K++) {
        int x;
        cin >> x;

        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                if (i + x <= n) {
                    int mask = 0;
                    for (int l = i; l < i + x; l++) mask += 1 << (l * n + j);
                    adj_list[K].emplace_back(mask);
                }
                if (x != 1 && j + x <= n) {
                    int mask = 0;
                    for (int l = j; l < j + x; l++) mask += 1 << (i * n + l);
                    adj_list[K].emplace_back(mask);
                }
            }
    }

    int count = 0;
    auto dfs = [&](auto &&self, int v = 0, int mask = 0) -> bool {
        if (v == k) return (hit & mask) == hit && !(miss & mask);

        for (int u : adj_list[v])
            if (!(mask & u)) count += self(self, v + 1, mask | u);
        return false;
    };
    dfs(dfs);
    cout << count;
}
