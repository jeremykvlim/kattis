#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> weights(n), team1(n / 2 + 1), team2(n - n / 2 + 1);
    for (int &w : weights) cin >> w;

    copy(weights.begin(), weights.begin() + n / 2, team1.begin());
    copy(weights.begin() + n / 2, weights.end(), team2.begin());
    int total1 = accumulate(team1.begin(), team1.end(), 0), total2 = accumulate(team2.begin(), team2.end(), 0);

    bool change;
    do {
        change = false;
        for (int i = 0; i < n / 2; i++)
            for (int j = 0; j < n - n / 2; j++) {
                int t1 = total1 - team1[i] + team2[j], t2 = total2 + team1[i] - team2[j];
                if (abs(t1 - t2) < abs(total1 - total2)) {
                    change = true;
                    swap(team1[i], team2[j]);
                    total1 = t1;
                    total2 = t2;
                }
            }
    } while (change);

    cout << min(total1, total2) << " " << max(total1, total2);
}
