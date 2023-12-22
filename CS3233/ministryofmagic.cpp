#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c, p;
    cin >> c >> p;

    vector<int> party(p);
    vector<queue<int>> rankings(p);
    for (int i = 0; i < p; i++) {
        cin >> party[i];

        for (int j = 0; j < c; j++) {
            int r;
            cin >> r;

            rankings[i].emplace(r);
        }
    }

    vector<int> votes(c + 1);
    multimap<int, int> candidates;
    for (int i = 0; i < p; i++) votes[rankings[i].front()] += party[i];
    for (int i = 1; i <= c; i++) candidates.emplace(votes[i], i);

    int total = accumulate(party.begin(), party.end(), 0), elected;
    vector<bool> eliminated(c + 1);
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < p; j++) {
            if (!eliminated[rankings[j].front()]) continue;
            while (eliminated[rankings[j].front()]) rankings[j].pop();

            int x = rankings[j].front();
            auto range = candidates.equal_range(votes[x]);
            for (auto it = range.first; it != range.second; it++)
                if (it->second == x) {
                    candidates.erase(it);
                    break;
                }

            votes[x] += party[j];
            candidates.emplace(votes[x], x);
        }

        auto it = prev(candidates.end());
        if (it->first > total / 2) {
            elected = it->second;
            break;
        }
        
        eliminated[candidates.begin()->second] = true;
        candidates.erase(candidates.begin());
    }

    cout << elected;
}
