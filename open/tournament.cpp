#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    while (cin >> k && k) {
        vector<pair<double, string>> knights(k);
        for (auto &[ability, name] : knights) cin >> name >> ability;
        sort(knights.begin(), knights.end());

        vector<double> mismatch(k + 1, 0);
        for (int i = 2; i <= k; i++) mismatch[i] = mismatch[i - 2] + (knights[i - 2].first - knights[i - 1].first) * (knights[i - 2].first - knights[i - 1].first);

        priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<>> pq;
        for (int i = 2; i <= k; i++) pq.emplace(mismatch[i] - mismatch[i - 2], make_pair(i - 2, i - 1));

        set<int> byes;
        for(int i = 0; i < k; i++) byes.emplace(i);

        while (k + byes.size() != bit_ceil((unsigned) k)) {
            auto [i, j] = pq.top().second;
            pq.pop();

            if (!byes.count(i) || !byes.count(j)) continue;

            byes.erase(i);
            byes.erase(j);

            auto it_i = byes.lower_bound(i), it_j = byes.lower_bound(j);
            if (it_i == byes.begin() || it_j == byes.end()) continue;
            i = *(prev(it_i));
            j = *it_j;

            pq.emplace(mismatch[i + 1] - mismatch[i] + mismatch[j + 1] - mismatch[j], make_pair(i, j));
        }

        cout << byes.size() << "\n";
        for (int i : byes) cout << knights[i].second << "\n";
    }
}
