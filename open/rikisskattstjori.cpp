#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, int>> ids(n);
    for (int i = 0; i < n; i++) {
        long long k;
        cin >> k;

        ids[i] = {k, i};
    }
    sort(ids.begin(), ids.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[ids[i].second] = i;

    auto build = [&](vector<int> r) {
        vector<vector<int>> indices;
        dynamic_bitset<> bs(n);
        vector<int> temp;
        for (int _ = 0; _ < ceil(sqrt(n)) && !r.empty(); _++) {
            bs.reset();
            temp.clear();
            for (int i : r) {
                bs.set(i);
                int j = bs.find_next(i);
                if (j < n) {
                    bs.reset(j);
                    temp.emplace_back(j);
                }
            }

            indices.emplace_back();
            for (int i = bs.find_first(); i < n; i = bs.find_next(i)) indices.back().emplace_back(i);
            r = temp;
        }
        return indices;
    };
    auto folders = build(rank);
    reverse(rank.begin(), rank.end());
    auto folders_rev = build(rank);

    for (int temp = folders.size(); auto &rev : folders_rev) {
        folders.resize(max(folders.size(), rev.size()));
        for (int s = temp; s < rev.size(); s++) folders[s].emplace_back(rev[s]);
    }

    for (auto &folder : folders) {
        for (int i : folder) cout << ids[i].first << (i == folder.back() ? ";" : " ");
        cout << "\n";
    }
}
