#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string f;
    cin >> n >> m >> f;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<int> sign(n, 0);
    for (int i = 0; i < n; i++) {
        if (f[i] == '+') sign[i] = 1;
        else if (f[i] == '-') sign[i] = -1;
    }

    queue<int> q;
    vector<bool> queued(n, false);
    for (int i = 0; i < n; i++)
        if (sign[i]) {
            q.emplace(i);
            queued[i] = true;
        }

    int count = 0;
    vector<int> s(n, 0), lazy(n, 0);
    while (!q.empty() && count <= 7e5) {
        int v = q.front();
        q.pop();

        queued[v] = false;

        if (!sign[v] || sign[v] * lazy[v] > 0) continue;

        count++;
        for (int u : adj_list[v]) {
            s[u] += sign[v];
            for (int t : adj_list[u]) {
                lazy[t] += sign[v];
                if (!queued[t] && sign[t] && sign[t] * lazy[t] < 1) {
                    q.emplace(t);
                    queued[t] = true;
                }
            }
        }
    }

    if (count > 7e5) {
        cout << "Impossible";
        exit(0);
    }

    for (int i = 0; i < n; i++)
        if (sign[i] && sign[i] * lazy[i] < 1) {
            cout << "Impossible";
            exit(0);
        }

    for (int si : s) cout << si << "\n";
}
