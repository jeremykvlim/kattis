#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    vector<int> points(n, 0), prev(w + 1, 0);
    vector<long long> total(n, 0), sum(w + 1, 0), rank(w + 1, 1);
    auto update = [&](int ci, int week, int sgn) {
        int p = points[ci];
        long long diff = week - prev[p];
        prev[p] = week;
        sum[p] += rank[p] * diff;
        total[ci] += sgn * sum[p];
    };

    for (int week = 0; week < w; week++) {
        int k;
        cin >> k;

        vector<int> c(k);
        for (int &i : c) {
            cin >> i;
            i--;

            update(i, week, 1);
            points[i]++;
            update(i, week, -1);
        }

        for (int i : c) rank[points[i] - 1]++;
    }

    for (int i = 0; i < n; i++) {
        update(i, w, 1);
        cout << fixed << setprecision(6) << (double) total[i] / w << "\n";
    }
}
