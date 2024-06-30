#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    for (int x = 1; x <= c; x++) {
        int n;
        cin >> n;

        int bound = -1;
        vector<pair<int, int>> messages(n);
        for (auto &[p, t] : messages) {
            cin >> p >> t;

            bound = max({bound, p, t});
        }
        sort(messages.begin(), messages.end(), [](auto p1, auto p2) {return p1.second < p2.second;});
        
        double l = 0, r = bound, m;
        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            m = l + (r - l) / 2;

            auto possible = [&]() -> bool {
                double l1 = messages[0].first - m, r1 = messages[0].first + m;
                for (int i = 1; i < n; i++) {
                    int t = messages[i].second - messages[i - 1].second;
                    double l2 = messages[i].first - m, r2 = messages[i].first + m;

                    if (l1 - t > r2 || r1 + t < l2) return false;

                    l1 = max(l1 - t, l2);
                    r1 = min(r1 + t, r2);
                }

                return true;
            };
            
            if (possible()) r = m;
            else l = m;
        }

        cout << "Case #" << x << ": " << fixed << setprecision(9) << m << "\n";
    }
}
