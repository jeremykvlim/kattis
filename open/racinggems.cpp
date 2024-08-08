#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R, w, h;
    cin >> n >> R >> w >> h;

    vector<pair<long long, long long>> gems(n);
    for (auto &[l, r] : gems) {
        long long x, y;
        cin >> x >> y;

        l = y - x * R;
        r = y + x * R;
    }
    sort(gems.begin(), gems.end());

    vector<long long> lis(n, LLONG_MAX);
    for (auto [l, r] : gems) *lower_bound(lis.begin(), lis.end(), r) = r;
    cout << lower_bound(lis.begin(), lis.end(), LLONG_MAX) - lis.begin();
}
