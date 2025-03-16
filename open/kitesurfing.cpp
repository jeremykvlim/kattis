#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s, d, t, n;
    cin >> s >> d >> t >> n;

    vector<int> l(n + 1), r(n + 1);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i];
    l[n] = s;

    unordered_map<int, long long> dist{{0, 0}};
    unordered_map<int, bool> visited;
    priority_queue<int, vector<int>, greater<>> pq;
    pq.emplace(0);
    while (!pq.empty()) {
        int v = pq.top();
        pq.pop();

        if (visited[v]) continue;

        visited[v] = true;

        auto relax = [&](int u, int v, long long t) {
            if (!dist.count(u) || dist[u] > dist[v] + t) {
                dist[u] = dist[v] + t;
                pq.emplace(u);
            }
        };
        int curr = lower_bound(begin(l), end(l), v) - begin(l);
        if ((l[curr] - v) / d > 1) {
            int jumps = max(0, (l[curr] - v) / d - 1);
            relax(v + jumps * d, v, (long long) jumps * t);
        } else {
            int next = curr;
            while (next < n && r[next] <= v + d) next++;
            relax(min(v + d, l[next]), v, t);
        }

        if (curr < n)
            for (int i = curr; i < n; i++) {
                int u = r[i] - d;
                if (v <= u && u <= l[curr]) relax(u, v, u - v);
            }
        else relax(s, v, s - v);
    }
    cout << dist[s];
}
