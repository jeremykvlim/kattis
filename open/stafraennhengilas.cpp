#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> curr(n), need(n);
    for (int &v : curr) cin >> v;
    for (int &v : need) cin >> v;

    vector<int> r(n);
    for (int i = 0; i < n; i++) r[i] = (need[i] - curr[i] + m) % m;

    vector<int> diff(n - 1);
    long long sum = r[0] + ((m - r[n - 1]) % m);
    for (int i = 0; i < n - 1; i++) sum += (diff[i] = (r[i + 1] - r[i] + m) % m);
    sum /= m;
    diff.emplace_back(r[0]);
    diff.emplace_back((m - r[n - 1]) % m);
    sort(diff.rbegin(), diff.rend());
    cout << sum * m - accumulate(diff.begin(), diff.begin() + sum, 0LL);
}
