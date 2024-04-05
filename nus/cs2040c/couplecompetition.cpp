#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> blocks(n);
    int highest = 0;
    for (int &h : blocks) {
        cin >> h;

        highest = max(highest, h);
    }

    vector<vector<int>> adj_list(n);
    auto construct = [&](stack<int> s, bool reverse) {
        for (int i = (reverse ? n - 1 : 0); (reverse ? ~i : i < n); (reverse ? i-- : i++)) {
            while (!s.empty() && blocks[s.top()] < blocks[i]) {
                adj_list[i].emplace_back(s.top());
                s.pop();
            }

            s.emplace(i);
        }
    };

    stack<int> s;
    construct(s, true);
    construct(s, false);

    vector<int> dist(n, -1);
    queue<int> q;
    for (int i = 0; i < n; i++)
        if (blocks[i] == highest) {
            q.emplace(i);
            dist[i] = 0;
        }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : adj_list[v])
            if (dist[u] == -1) {
                dist[u] = dist[v] + 1;
                q.emplace(u);
            }
    }

    for (auto d : dist) cout << d << " ";
}
