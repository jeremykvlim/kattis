#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> microbes(n);
    for (int i = 0; i < n; i++) {
        cin >> microbes[i][0] >> microbes[i][1];

        microbes[i][2] = i;
    }
    sort(microbes.begin(), microbes.end());

    vector<vector<array<int, 3>>> rows;
    for (int i = 0; i < n; i++) {
        auto it = lower_bound(rows.begin(), rows.end(), microbes[i][1], [](const auto &a, int s) {return a.back()[1] > s;});

        if (it == rows.end()) rows.emplace_back(1, microbes[i]);
        else it->emplace_back(microbes[i]);
    }

    vector<int> R(n);
    for (int r = 0; r < rows.size(); r++)
        for (auto [p, s, i]: rows[r]) R[i] = r + 1;
    for (int r : R) cout << r << " ";
}
