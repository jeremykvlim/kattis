#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> weights(n), team1((n >> 1) + 1), team2(n - (n >> 1) + 1);
    for (int &w : weights) cin >> w;

    int total1 = accumulate(weights.begin(), weights.begin() + (n >> 1), 0), total2 = accumulate(weights.begin() + (n >> 1), weights.end(), 0);

    copy(weights.begin(), weights.begin() + (n >> 1), team1.begin());
    copy(weights.begin() + (n >> 1), weights.end(), team2.begin());

    bool done;
    do {
        done = true;
        for (int i = 0; i < n >> 1; i++)
            for (int j = 0; j < n - (n >> 1); j++) {
                int t1 = total1 - team1[i] + team2[j], t2 = total2 + team1[i] - team2[j];
                if (abs(t1 - t2) < abs(total1 - total2)) {
                    done = false;
                    swap(team1[i], team2[j]);
                    total1 = t1;
                    total2 = t2;
                }
            }
    } while (!done);

    cout << min(total1, total2) << " " << max(total1, total2);
}
