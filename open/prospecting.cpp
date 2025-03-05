#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    vector<vector<pair<int, int>>> adj_list(n);
    for (int i = 1; i < n; i++) {
        int p, y;
        cin >> p >> x[i] >> y;

        adj_list[p].emplace_back(i, y);
        if (x[i] == -1) x[i] = 1e6;
    }

    auto dfs1 = [&](auto &&self, int v = 0) -> int {
        int a = 0;
        for (auto [u, w] : adj_list[v]) a += max(w - 1, w - x[u] + self(self, u));
        return a;
    };
    cout << dfs1(dfs1) + 1 << " ";

    int least, money;
    auto eval = [&](__gnu_pbds::priority_queue<pair<int, int>, less<>, pairing_heap_tag> &pq, bool last = true) -> void {
        while (!pq.empty() && (last || money <= 0 || money + pq.top().first >= least)) {
            auto [l, gain] = pq.top();
            pq.pop();

            least = min(least, money + l);
            money += gain;
        }
    };

    auto dfs2 = [&](auto &&self, int v = 0) -> __gnu_pbds::priority_queue<pair<int, int>, less<>, pairing_heap_tag> {
        __gnu_pbds::priority_queue<pair<int, int>, less<>, pairing_heap_tag> pq;
        pq.push({x[v], x[v]});
        for (auto [u, w] : adj_list[v]) {
            auto temp = self(self, u);
            least = money = -w;
            eval(temp, false);
            if (money > 0) temp.push({least, money});
            pq.join(temp);
        }
        return pq;
    };

    auto pq = dfs2(dfs2);
    least = money = 0;
    eval(pq);
    cout << -least;
}
