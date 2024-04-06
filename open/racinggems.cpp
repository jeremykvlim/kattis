#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, w, h;
    cin >> n >> r >> w >> h;

    vector<pair<long long, long long>> gems(n);
    for (int i = 0; i < n; i++) {
        long long x, y;
        cin >> x >> y;

        gems[i] = {y - x * r, y + x * r};
    }
    sort(gems.begin(), gems.end());

    vector<long long> lis(n, LLONG_MAX);
    for (auto [x, y] : gems) *lower_bound(lis.begin(), lis.end(), y) = y;

    cout << lower_bound(lis.begin(), lis.end(), LLONG_MAX) - lis.begin();
}
