#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> names(m);
    for (auto &name : names) cin >> name;

    vector<int> count(m, 0);
    vector<vector<int>> prefers(n, vector<int>(m)), voters(m);
    for (int v = 0; v < n; v++) {
        for (int &choice : prefers[v]) {
            cin >> choice;
            choice--;
        }
        voters[prefers[v][0]].emplace_back(v);
        count[prefers[v][0]]++;
    }

    int votes = 0, winner = -1;
    for (int i = 0; i < m; i++)
        if (votes < count[i]) {
            votes = count[i];
            winner = i;
        }

    if (votes > n / 2) {
        cout << names[winner];
        exit(0);
    }

    vector<int> pos(n, 0), gain(m, 0);
    vector<bool> lost(m, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < m; i++) pq.emplace(count[i], -i);
    while (m > 1) {
        int loser;
        for (;;) {
            auto [c, i] = pq.top();
            pq.pop();

            if (lost[-i] || c != count[-i]) continue;
            loser = -i;
            break;
        }
        lost[loser] = true;
        m--;

        vector<int> affected;
        for (int v : voters[loser]) {
            int p = pos[v] + 1;
            for (; lost[prefers[v][p]]; p++);
            pos[v] = p;

            int i = prefers[v][p];
            voters[i].emplace_back(v);
            if (!gain[i]++) affected.emplace_back(i);
        }

        for (int i : affected) {
            count[i] += gain[i];
            gain[i] = 0;
            pq.emplace(count[i], -i);
            if (votes < count[i]) {
                votes = count[i];
                winner = i;
            }
        }

        if (votes > n / 2) {
            cout << names[winner];
            exit(0);
        }
    }

    for (int i = 0; i < lost.size(); i++)
        if (!lost[i]) {
            cout << names[i];
            exit(0);
        }
}
