#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    n = min(n, 28);

    vector<pair<int, int>> puzzles(n);
    for (auto &[x, y] : puzzles) cin >> x >> y;

    auto meet_in_the_middle = [&](int l, int r) {
        vector<pair<pair<int, int>, long long>> subsets{{{0, 0}, 0}};
        for (int i = l; i < r; i++) {
            int s = subsets.size();
            subsets.resize(3 * s);
            auto [x, y] = puzzles[i];
            for (int j = 0; j < s; j++) {
                subsets[j + s] = subsets[j + 2 * s] = subsets[j];
                subsets[j + s].first.first += x;
                subsets[j + s].first.second += y;
                subsets[j + s].second |= 1LL << (i << 1);
                subsets[j + 2 * s].first.first -= x;
                subsets[j + 2 * s].first.second -= y;
                subsets[j + 2 * s].second |= 1LL << (i << 1 | 1);
            }
            inplace_merge(subsets.begin(), subsets.begin() + s, subsets.begin() + 2 * s);
            inplace_merge(subsets.begin(), subsets.begin() + 2 * s, subsets.end());
        }
        return subsets;
    };
    auto subsets_l = meet_in_the_middle(0, n / 2), subsets_r = meet_in_the_middle(n / 2, n);

    for (int i = 0, j = 0; i < subsets_l.size(); i++) {
        while (j < subsets_r.size() && subsets_l[i].first > subsets_r[j].first) j++;
        if (j == subsets_r.size()) break;

        if (subsets_l[i].first == subsets_r[j].first && (subsets_l[i].second || subsets_r[j].second)) {
            vector<int> present1, present2;
            auto mask = subsets_l[i].second | subsets_r[j].second;
            for (int k = 0; k < n; k++) {
                int p = (mask >> (k << 1)) & 3;
                if (p == 1) (k < n / 2 ? present1 : present2).emplace_back(k + 1);
                else if (p == 2) (k < n / 2 ? present2 : present1).emplace_back(k + 1);
            }
            cout << "no\n";
            auto print = [&](vector<int> &present) {
                cout << present.size() << " ";
                for (int k : present) cout << k << " ";
                cout << "\n";
            };
            print(present1);
            print(present2);
            exit(0);
        }
    }
    cout << "yes";
}
