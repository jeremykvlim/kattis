#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int D;
    cin >> D;

    unordered_map<int, list<vector<int>>> items;
    int count = 0;
    while (D--) {
        int t;
        cin >> t;

        if (t == 1) {
            int f;
            cin >> f;

            if (items[f].empty()) items[f].emplace_back();
            items[f].back().emplace_back(count++);
        } else {
            int s, d;
            cin >> s >> d;

            items[d].splice(items[d].end(), items[s]);
        }
    }

    vector<int> day(count);
    for (auto &[id, list] : items)
        for (auto &v : list)
            for (int &x : v) day[x] = id;

    for (int id : day) cout << id << "\n";
}
