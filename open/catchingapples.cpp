#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> apples(n);
    for (int i = 0; i < n; i++) {
        int t, x;
        cin >> t >> x;

        apples[i] = {x - t, x + t, i};
    }
    sort(apples.begin(), apples.end(), [&](auto a1, auto a2) { return a1[1] != a2[1] ? a1[1] < a2[1] : a1[0] > a2[0]; });

    int k = 0;
    vector<int> a(n);
    set<pair<int, int>> s;
    for (auto [l, r, i] : apples) {
        auto it = s.lower_bound({l, 0});

        int id;
        if (it == s.end()) id = ++k;
        else {
            id = it->second;
            s.erase(it);
        }

        s.emplace(l, a[i] = id);
    }

    cout << k << "\n";
    for (int ai : a) cout << ai << " ";
}
