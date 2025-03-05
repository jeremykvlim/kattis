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

        long long least, h;
        auto eval = [&](__gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag> &pq, bool last = true) -> void {
            while (!pq.empty() && (last || h <= 0 || h + pq.top().first >= least)) {
                auto [l, gain] = pq.top();
                pq.pop();

                least = min(least, h + l);
                h += gain;
            }
        };

        auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> __gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag> {
            __gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag> pq;
            if (v == t - 1) pq.push({0, 1e12});
            for (int u : adj_list[v])
                if (u != prev) {
                    auto temp = self(self, u, v);
                    pq.join(temp);
                }

            least = 0, h = hp[v];
            eval(pq, false);
            if (h > 0) pq.push({least, h});
            return pq;
        };
        auto pq = dfs(dfs);

        least = 0, h = 0;
        eval(pq);
        cout << (least || h < (long long) 1e12 ? "trapped\n" : "escaped\n");
    }
}
