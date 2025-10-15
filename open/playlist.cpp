#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int count = 0;
    vector<string> artists(n);
    unordered_map<string, int> indices;
    vector<vector<int>> adj_list_regular(n), adj_list_transpose(n);
    for (int i = 0; i < n; i++) {
        int t;
        cin >> artists[i] >> t;

        if (!indices.count(artists[i])) indices[artists[i]] = count++;
        while (t--) {
            int s;
            cin >> s;

            adj_list_regular[i].emplace_back(s - 1);
            adj_list_transpose[s - 1].emplace_back(i);
        }
    }

    int k = 11;
    vector<vector<int>> masks(10);
    for (int mask = 1; mask < 1 << k; mask++) {
        int p = popcount((unsigned) mask);
        if (p <= 9) masks[p].emplace_back(mask);
    }

    vector<__int128> dp(1 << k);
    vector<int> random(count), color(n);
    vector<vector<pair<int, int>>> prev(1 << k, vector<pair<int, int>>(n, {-1, -1}));
    mt19937 rng(random_device{}());
    auto t0 = steady_clock::now();
    while (duration<double>(steady_clock::now() - t0).count() < 4) {
        fill(dp.begin(), dp.end(), 0);
        for (int i = 0; i < count; i++) random[i] = rng() % k;
        for (int i = 0; i < n; i++) color[i] = random[indices[artists[i]]];
        for (int i = 0; i < n; i++) dp[1 << color[i]] |= (__int128) 1 << i;
        
        for (int p = 2; p <= 9; p++)
            for (int m1 : masks[p])
                for (int v = 0; v < n; v++) {
                    int m0 = 1 << color[v];
                    if (m0 & m1) {
                        int m2 = m1 ^ m0;
                        for (int u : adj_list_transpose[v]) 
                            if ((dp[m2] >> u) & 1) {
                                dp[m1] |= (__int128) 1 << v;
                                prev[m1][v] = {m2, u};
                                if (p == 9) {
                                    vector<int> path;
                                    for (int i = 9; i; i--) {
                                        path.emplace_back(v);
                                        tie(m1, v) = prev[m1][v];
                                    }
                                    reverse(path.begin(), path.end());
                                    for (int t : path) cout << t + 1 << " ";
                                    exit(0);
                                }
                            }
                    }
                }

    }
    cout << "fail";
}
