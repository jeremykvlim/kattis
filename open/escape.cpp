#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, t;
        cin >> n >> t;

        vector<int> hp(n);
        for (int &h : hp) cin >> h;

        vector<vector<int>> adj_list(n);
        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;

            adj_list[u - 1].emplace_back(v - 1);
            adj_list[v - 1].emplace_back(u - 1);
        }

        vector<__gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag>> pqs(n);
        pqs[t - 1].push({0, 1e12});

        long long least, h;
        auto eval = [&](int v = 0) -> void {
            while (!pqs[v].empty() && (h <= 0 || h + pqs[v].top().first >= least)) {
                auto [l, gain] = pqs[v].top();
                pqs[v].pop();

                least = min(least, h + l);
                h += gain;
            }
        };

        auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
            for (int u : adj_list[v])
                if (u != prev) {
                    self(self, u, v);
                    pqs[v].join(pqs[u]);
                }

            least = 0, h = hp[v];
            eval(v);
            if (h >= 0) pqs[v].push({least, h});
        };
        dfs(dfs);

        least = h = 0;
        eval();
        cout << (least || h < (long long) 1e12 ? "trapped\n" : "escaped\n");
    }
}
