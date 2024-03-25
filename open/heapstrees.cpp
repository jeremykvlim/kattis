#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> value(n + 1), parent(n + 1), index(n + 1);
    for (int i = 1; i <= n; i++) cin >> value[i] >> parent[i];
    iota(index.begin(), index.end(), 0);

    vector<multiset<int>> sets(n + 1);
    for (int i = n; i >= 1; i--) {
        auto it = sets[index[i]].upper_bound(value[i] - 1);
        if (it != sets[index[i]].end()) sets[index[i]].erase(it);
        sets[index[i]].insert(value[i]);
      
        if (sets[index[i]].size() < sets[index[parent[i]]].size()) sets[index[parent[i]]].insert(sets[index[i]].begin(), sets[index[i]].end());
        else {
            sets[index[i]].insert(sets[index[parent[i]]].begin(), sets[index[parent[i]]].end());
            index[parent[i]] = index[i];
        }
    }

    cout << sets[index[0]].size();
}
