#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> team(n);
    tree<pair<pair<int, int>, int>, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update> scores;
    for (int i = 0; i < n; i++) scores.insert({{0, 0}, i});
    while (m--) {
        int t, p;
        cin >> t >> p;

        auto [a, b] = team[--t];
        scores.erase({team[t], t});
        scores.insert({{a - 1, b + p}, t});
        team[t] = {a - 1, b + p};

        cout << scores.order_of_key({team[0], 0}) + 1 << "\n";
    }
}
