#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> times(n), pref(n);

        for (int i = 0; i < n; i++) {
            double p;
            cin >> times[i] >> p;
            pref[i] = i ? pref[i - 1] + round(p * 100) : round(p * 100);
        }

        int profit = -1, start = times.front(), end = times.back();
        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++) {
                int curr = pref[j] - (i ? pref[i - 1] : 0) - (times[j] - times[i] + 1) * 8;
                if (curr > profit || (curr == profit && times[j] - times[i] < end - start)) tie(start, end, profit) = make_tuple(times[i], times[j], curr);
            }

        if (profit > 0) cout << fixed << setprecision(2) << profit / 100.0 << " " << start << " " << end << "\n";
        else cout << "no profit\n";
    }
}
