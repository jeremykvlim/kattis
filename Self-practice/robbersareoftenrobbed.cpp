#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> stars(n);
    for (int &s : stars) cin >> s;
    sort(stars.rbegin(), stars.rend());
    
    pair<int, int> optimal = {0, 0};
    for (int i = 1; i <= stars[0]; i++) {
        int sacks = 0;
        vector<int> remaining, unvisited(stars.begin() + min(n, k), stars.end());
        for (int j = 0; j < min(n, k); j++) {
            sacks += stars[j] / i;
            if (stars[j] % i) remaining.emplace_back(stars[j] % i);
        }

        if (sacks >= k) optimal = max(optimal, {i * k / 2, -i * k / 2});
        else {
            remaining.insert(remaining.end(), unvisited.begin(), unvisited.end());
            sort(remaining.rbegin(), remaining.rend());
            remaining.resize(k - sacks);
            remaining.insert(remaining.end(), sacks, i);
        }

        remaining.resize(k);
        sort(remaining.rbegin(), remaining.rend());
        optimal = max(optimal, {accumulate(remaining.begin() + k / 2, remaining.end(), 0), -accumulate(remaining.begin(), remaining.begin() + k / 2, 0)});
    }
    
    cout << optimal.first << " " << -optimal.second;
}
