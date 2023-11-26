#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<int> candidates(n);
    int highest = 0, final = 0;

    for (int i = 0; i < n; i++) {
        cin >> candidates[i];
        highest = max(highest, candidates[i]);
    }

    for (int i = 0; i < n; i++)
        if (candidates[i] == highest) final++;

    vector<pair<int, int>> q;
    int count = 1;
    for (int i = 1; i < n; i++) {
        if (candidates[i] == candidates[i - 1]) count++;
        else {
            q.emplace_back(candidates[i - 1], count);
            count = 1;
        }
    }

    q.emplace_back(candidates[n - 1], count);
    vector<vector<int>> result;

    for (;;) {
        int remaining = 0, last = -1;
        vector<int> leavers;
        vector<pair<int, int>> q_new;

        for (int i = 0; i < q.size(); i++) {
            if (i > 0 && q[i - 1].first > q[i].first) {
                q[i].second--;
                leavers.emplace_back(q[i].first);
                remaining++;
            }

            if (i < q.size() - 1 && q[i + 1].first > q[i].first && q[i].second > 0) {
                q[i].second--;
                leavers.emplace_back(q[i].first);
                remaining++;
            }

            if (q[i].second == 0) continue;

            if (q[i].first == last) {
                q_new[q_new.size() - 1].second += q[i].second;
                continue;
            }

            last = q[i].first;
            q_new.emplace_back(q[i]);
        }

        if (remaining == 0) break;

        result.emplace_back(leavers);
        swap(q, q_new);
        q_new.clear();
    }

    cout << result.size() << endl;
    for (auto& i : result) {
        for (auto j : i) cout << j << " ";
        cout << "\n";
    }

    for (int i = 0; i < final; i++)
        cout << highest << " ";
    cout << endl;
}
