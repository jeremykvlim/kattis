#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> hats(n);
    for (auto &[w, h] : hats) cin >> w >> h;
    sort(hats.begin(), hats.end(), [&](auto &p1, auto &p2) { return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second; });

    vector<int> lis;
    for (auto [w, h] : hats) {
        auto it = lower_bound(lis.begin(), lis.end(), h);
        if (it != lis.end()) *it = h;
        else lis.emplace_back(h);
    }
    cout << lis.size();
}
