#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<unordered_map<int, bool>> adj_matrix(n + 1);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_matrix[u][v] = adj_matrix[v][u] = true;
    }

    deque<int> dq;
    dq.emplace_back(n);
    for (int i = n - 1; i; i--) {
        if (adj_matrix[dq.front()].count(i)) dq.emplace_front(i);
        else if (!adj_matrix[dq.back()].count(i)) dq.emplace_back(i);
        else if (adj_matrix[dq.front()].count(dq.back())) {
            dq.emplace_front(dq.back());
            dq.pop_back();
            dq.emplace_front(i);
        }
        else {
            dq.emplace_back(dq.front());
            dq.pop_front();
            dq.emplace_back(i);
        }
    }

    if (dq.back() == 1) reverse(dq.begin(), dq.end());
    for (int ai : dq) cout << ai << " ";
}
