#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> candidates(n);
    int highest = 0, count;
    for (auto &v : candidates) {
        cin >> v;
        if (v > highest) {
            highest = v;
            count = 1;
        } else if (v == highest) count++;
    }

    vector<pair<int, int>> q;
    int same = 1;
    for (int i = 1; i < n; i++) {
        if (candidates[i] == candidates[i - 1]) same++;
        else {
            q.emplace_back(candidates[i - 1], same);
            same = 1;
        }
    }

    q.emplace_back(candidates[n - 1], same);
    vector<vector<int>> m;

    for (;;) {
        int remaining = 0;
        vector<int> leavers;
        vector<pair<int, int>> temp;

        for (int i = 0; i < q.size(); i++) {
            auto process = [&] {
                leavers.emplace_back(q[i].first);
                q[i].second--;
                remaining++;
            };

            if (i && q[i - 1].first > q[i].first) process();
            if (i < q.size() - 1 && q[i + 1].first > q[i].first && q[i].second) process();

            if (!q[i].second) continue;
            if (!temp.empty() && q[i].first == temp.back().first) {
                temp.back().second += q[i].second;
                continue;
            }

            temp.emplace_back(q[i]);
        }

        if (!remaining) break;

        m.emplace_back(leavers);
        q = temp;
    }

    cout << m.size() << "\n";
    for (auto &i : m) {
        for (auto j : i) cout << j << " ";
        cout << "\n";
    }

    while (count--) cout << highest << " ";
}
