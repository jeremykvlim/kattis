#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> t_lap(n), c(n);
    for (int i = 0; i < n; i++) cin >> t_lap[i] >> c[i];

    queue<int> q;
    for (int i = 0; i < n; i++) q.emplace(i);

    vector<int> t_complete(n, -1), completed(n, 0), t_curr = t_lap;
    while (!q.empty()) {
        int t = t_curr[q.front()];

        vector<int> group;
        while (!q.empty() && t_curr[q.front()] == t) {
            group.emplace_back(q.front());
            q.pop();
        }

        vector<int> restart;
        for (int i : group)
            if (++completed[i] == c[i]) t_complete[i] = t;
            else restart.emplace_back(i);
        sort(restart.begin(), restart.end());

        int t_prev = q.empty() ? t : t_curr[q.back()];
        for (int i : restart) {
            t_curr[i] = t_prev = max(t_prev, t + t_lap[i]);
            q.emplace(i);
        }
    }
    for (int t : t_complete) cout << t << "\n";
}
