#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int b, p;
    cin >> b >> p;
    
    vector<int> digits(p);
    for (auto &x : digits) cin >> x;

    vector<vector<pair<int, int>>> prev{{{0, 0}}, {}};
    for (int i = p - 1; i >= 0; i--) {
        vector<vector<pair<int, int>>> curr(2);
        for (int j = 0; j < 2; j++) {
            sort(prev[j].begin(), prev[j].end());
            int min = INT_MAX;
            for (auto [alice, bob]: prev[j]) {
                if (bob >= min) continue;
                min = bob;
                curr[0].push_back({alice + (digits[i] + j), bob});
                curr[1].push_back({alice, bob + (b - digits[i] - j)});
            }
        }
        prev.swap(curr);
    }

    int transfers = INT_MAX;
    for (auto i : prev)
        for (auto [alice, bob] : i) transfers = min(transfers, max(alice, bob));
    cout << transfers;
}
