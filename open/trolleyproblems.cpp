#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> req(n);
        for (int &s : req) cin >> s;

        int l = 0, r = 410, m;
        vector<int> stops;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            vector<int> s;
            auto valid = [&]() {
                int i = 0;
                while (i < n && req[i] <= (m / 20) + 1) i++;

                for (int j = 1, a = 1; i < n; j++) {
                    pair<int, int> latest{-1, -1};
                    for (int stop = a + 1; stop <= 31; stop++) {
                        int k = 4 * (stop - 1) + 10 * (j - 1);
                        if (k > m) continue;

                        int w = (m - k) / 20, lb = stop - w, ub = stop + w;
                        if (lb <= req[i] && req[i] <= ub) latest = max(latest, make_pair(ub, stop));
                    }
                    if (latest == make_pair(-1, -1)) return false;

                    while (i < n && req[i] <= latest.first) i++;
                    s.emplace_back(a = latest.second);
                }

                return true;
            };

            if (valid()) {
                stops = s;
                r = m;
            } else l = m;
        }

        cout << r << "\n" << stops.size() << " ";
        for (int s : stops) cout << s << " ";
        cout << "\n";
    }
}
