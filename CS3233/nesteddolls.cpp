#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int m;
        cin >> m;

        vector<pair<int, int>> dolls(m);
        for (auto &[w, h] : dolls) cin >> w >> h;
        sort(dolls.rbegin(), dolls.rend(), [&](auto p1, auto p2) {return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second;});

        vector<int> dp;
        for (auto &[w, h] : dolls) {
            auto it = upper_bound(dp.begin(), dp.end(), h);
            if (it == dp.end()) dp.emplace_back(h);
            else *it = h;
        }

        cout << dp.size() << "\n";
    }
}
