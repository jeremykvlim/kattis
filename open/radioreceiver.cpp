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

        auto possible = [&](double m) -> bool {
            double l1 = messages[0].first - m, r1 = messages[0].first + m;
            for (int i = 1; i < n; i++) {
                int t = messages[i].second - messages[i - 1].second;
                double l2 = messages[i].first - m, r2 = messages[i].first + m;

                if (l1 - t > r2 || r1 + t < l2) return false;

                if (l1 - t >= l2 && r1 + t <= r2) {
                    l1 -= t;
                    r1 += t;
                } else if (l1 - t < l2 && r1 + t > r2) {
                    l1 = l2;
                    r1 = r2;
                } else {
                    l1 = max(l1 - t, l2);
                    r1 = min(r1 + t, r2);
                }
            }

            return true;
        };

        int count = 0;
        double l = 0, r = bound, m;
        while (l + 1e-9 < r && r / l > 1 + 1e-9) {
            m = l + (r - l) / 2;

            if (possible(m)) r = m;
            else l = m;

            count++;
        }

        cout << "Case #" << x << ": " << fixed << setprecision(9) << m << "\n";
    }
}
