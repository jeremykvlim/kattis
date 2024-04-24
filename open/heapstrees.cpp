#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> v(n + 1), p(n + 1), indices(n + 1);
    for (int i = 1; i <= n; i++) cin >> v[i] >> p[i];
    iota(indices.begin(), indices.end(), 0);

    vector<multiset<int>> sets(n + 1);
    for (int i = n; i; i--) {
        auto it = sets[indices[i]].upper_bound(v[i] - 1);
        if (it != sets[indices[i]].end()) sets[indices[i]].erase(it);
        sets[indices[i]].emplace(v[i]);

        if (sets[indices[i]].size() < sets[indices[p[i]]].size()) sets[indices[p[i]]].insert(sets[indices[i]].begin(), sets[indices[i]].end());
        else {
            sets[indices[i]].insert(sets[indices[p[i]]].begin(), sets[indices[p[i]]].end());
            indices[p[i]] = indices[i];
        }
    }

    cout << sets[indices[0]].size();
}
