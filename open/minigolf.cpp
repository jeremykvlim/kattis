#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, h;
    cin >> p >> h;

    vector<vector<int>> score(p, vector<int>(h));
    vector<long long> sum(p, 0);
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < h; j++) {
            cin >> score[i][j];

            sum[i] += score[i][j];
        }
        sort(score[i].rbegin(), score[i].rend());
    }

    for (int i = 0; i < p; i++) {
        vector<pair<int, int>> changes;
        int r = 0;
        for (int j = 0; j < p; j++) {
            auto si = sum[i], sj = sum[j], prev = (long long) 1e9;
            if (si >= sj) r++;

            for (int k = 0, l = 0; k < h && l < h; score[i][k] > score[j][l] ? k++ : l++) {
                bool b = si >= sj;

                int curr = max(score[i][k], score[j][l]);
                si -= (prev - curr) * k;
                sj -= (prev - curr) * l;

                if (!b && si >= sj) changes.emplace_back(curr + (si - sj) / (l - k), 1);
                else if (b && si < sj) changes.emplace_back(curr + (sj - si - 1) / (k - l), -1);
                prev = curr;
            }
        }
        sort(changes.rbegin(), changes.rend());

        int rank = r;
        for (auto [s, delta] : changes) {
            r += delta;
            rank = min(rank, r);
        }
        cout << rank << "\n";
    }
}
