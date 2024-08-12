#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long s;
    cin >> n >> s;

    vector<vector<int>> adj_list(n + 1);
    vector<long long> x(n + 1);
    x[0] = s;
    for (int i = 1; i <= n; i++) {
        int p;
        cin >> x[i] >> p;

        adj_list[p].emplace_back(i);
    }

    auto dfs = [&](auto &&self, int v = 0) -> __gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag> {
        __gnu_pbds::priority_queue<pair<long long, long long>, less<>, pairing_heap_tag> pq;
        for (int u : adj_list[v]) {
            auto temp = self(self, u);
            pq.join(temp);
        }

        if (x[v] > 0) pq.push({0, x[v]});
        else {
            auto lv = x[v], xv = x[v];
            while (!pq.empty()) {
                auto [li, xi] = pq.top();

                if (xv <= 0) {
                    pq.pop();
                    lv = min(lv, xv + li);
                    xv += xi;
                } else if (li >= lv) {
                    pq.pop();
                    xv += xi;
                } else break;
            }

            if (xv > 0) pq.push({lv, xv});
        }

        return pq;
    };

    auto pq = dfs(dfs);
    auto profit = 0LL;
    while (!pq.empty()) {
        auto [li, xi] = pq.top();
        pq.pop();

        if (profit < -li) break;

        profit += xi;
    }

    cout << profit - s;
}
