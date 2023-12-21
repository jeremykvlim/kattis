#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<pair<int, int>> rain(n + 2);
        for (int i = 1; i <= n; i++) cin >> rain[i].first >> rain[i].second;
        rain[0] = {INT_MIN, INT_MAX};
        rain[n + 1] = {INT_MAX, INT_MAX};

        vector<int> prev(n + 2, 0), next(n + 2, 0);
        for (int i = 1, j = 0, k = n + 1; i < n + 2; j = i, k = n + 1 - i++) {
            while (rain[i].second > rain[j].second) j = prev[j];
            prev[i] = j;
            while (rain[n + 1 - i].second > rain[k].second) k = next[k];
            next[n + 1 - i] = k;
        }

        int m;
        cin >> m;

        while (m--) {
            int y, x;
            cin >> y >> x;

            auto Y = upper_bound(rain.begin(), rain.end(), make_pair(y, -1)), X = upper_bound(rain.begin(), rain.end(), make_pair(x, -1));
            if (Y->first == y && X->first == x) {
                auto Z = rain.begin() + prev[X - rain.begin()];
                if (Y != Z) cout << "false\n";
                else if (X - Y == x - y) cout << "true\n";
                else cout << "maybe\n";
            } else if (Y->first == y) {
                auto Z = rain.begin() + next[Y - rain.begin()];
                if (Z->first <= x) cout << "false\n";
                else cout << "maybe\n";
            } else if (X->first == x) {
                auto Z = rain.begin() + prev[X - rain.begin()];
                if (Z->first >= y) cout << "false\n";
                else cout << "maybe\n";
            } else cout << "maybe\n";
        }

        cout << "\n";
    }
}
