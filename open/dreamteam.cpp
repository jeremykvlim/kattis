#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, q;
    cin >> n >> p >> q;

    vector<pair<int, int>> engineers(n);
    for (auto &[a, s] : engineers) cin >> a;
    for (auto &[a, s] : engineers) cin >> s;
    sort(engineers.begin(), engineers.end());

    int cost = INT_MAX;
    for (int l = 0, r = 0; l < n; r = ++l) {
        while (r < n && engineers[r].first - engineers[l].first <= p) r++;

        int size = r - l;
        if (size < 12) continue;

        vector<int> s(size);
        for (int i = l; i < r; i++) s[i - l] = engineers[i].second;
        sort(s.begin(), s.end());

        int sum = 0;
        for (int i = 0; i < 12; i++) sum += s[i];

        if (s[11] - s[0] <= q) cost = min(cost, sum);
        for (int i = 12; i < size; i++) {
            sum += s[i] - s[i - 12];
            if (s[i] - s[i - 11] <= q) cost = min(cost, sum);
        }
    }

    if (cost == INT_MAX) cout << "NO";
    else cout << "YES\n" << cost;
}
