#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        auto mgcd = 0LL;
        vector<pair<long long, int>> a;
        while (n--) {
            long long ai;
            cin >> ai;

            mgcd = max(mgcd, ai);
            for (int i = 0; i < a.size(); i++) {
                a[i].first = __gcd(ai, a[i].first);
                a[i].second++;
                mgcd = max(mgcd, a[i].first * a[i].second);
                if (i && a[i].first == a[i - 1].first) {
                    a.erase(a.begin() + i);
                    i--;
                }
            }
            if (a.empty() || a.back().first != ai) a.emplace_back(ai, 1);
        }
        cout << mgcd << "\n";
    }
}
