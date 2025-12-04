#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> s(n);
    for (int &si : s) cin >> si;

    int g = s[0];
    for (int i = 1; i < n; i++) g = __gcd(g, s[i]);

    int mod = 1e5;
    for (int &si : s) mod = min(mod, si /= g);

    vector<long long> dist(mod, 1e18);
    dist[0] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    pq.emplace(0, 0);
    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d != dist[v]) continue;

        for (int w : s) {
            int u = (v + w) % mod;
            if (dist[u] > d + w) {
                dist[u] = d + w;
                pq.emplace(d + w, u);
            }
        }
    }

    while (m--) {
        long long b;
        cin >> b;

        bool a = false;
        if (!(b % g)) {
            b /= g;
            a = dist[b % mod] <= b;
        }
        cout << a << " ";
    }
}
