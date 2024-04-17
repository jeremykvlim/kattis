#include <bits/stdc++.h>
using namespace std;

void relax(int u, int v, long long t, unordered_map<int, long long> &time, priority_queue<int, vector<int>, greater<>> &pq) {
    if (!time.count(u)) time[u] = LLONG_MAX;

    if (time[u] > time[v] + t) {
        time[u] = time[v] + t;
        pq.emplace(u);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s, d, t, n;
    cin >> s >> d >> t >> n;

    vector<int> l(n + 1), r(n + 1);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i];
    l[n] = s;

    unordered_map<int, long long> time{{0, 0}};
    unordered_map<int, bool> visited;
    priority_queue<int, vector<int>, greater<>> pq;
    pq.emplace(0);
    while (!pq.empty()) {
        int v = pq.top();
        pq.pop();

        if (visited[v]) continue;

        visited[v] = true;
        int curr = lower_bound(begin(l), end(l), v) - begin(l);
        if ((l[curr] - v) / d > 1) {
            int jumps = max(0, (l[curr] - v) / d - 1);
            relax(v + jumps * d, v, (long long) jumps * t, time, pq);
        } else {
            int next = curr;
            while (next < n && r[next] <= v + d) next++;
            relax(min(v + d, l[next]), v, t, time, pq);
        }

        if (curr < n)
            for (int i = curr; i < n; i++) {
                int u = r[i] - d;
                if (v <= u && u <= l[curr]) relax(u, v, u - v, time, pq);
            }
        else relax(s, v, s - v, time, pq);
    }

    cout << time[s];
}
