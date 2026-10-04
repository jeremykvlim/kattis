#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, int>> ids(n);
    for (int i = 0; i < n; i++) {
        cin >> ids[i].first;

        ids[i].second = i;
    }
    sort(ids.begin(), ids.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[ids[i].second] = i;

    vector<pair<int, int>> left(n), right(n);
    for (int i = 0; i < n; i++) left[i] = {rank[i], n};
    for (int i = 0; i < n; i++) right[i] = {rank[n - 1 - i], n};

    vector<int> folder;
    auto build = [&](vector<pair<int, int>> &order) {
        folder.clear();
        int size = 0;
        for (int i = 0; i < order.size(); i++) {
            auto [ri, j] = order[i];
            int f = folder.size();
            if (j + 1 >= f && (!f || folder.back() < ri)) {
                folder.emplace_back(ri);
                continue;
            }

            int l = j < f ? j - 1 : f - 2, r = l + 1, m;
            for (; l >= 0 && folder[l] > ri; l = max(-1, 2 * l - r));
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (folder[m] > ri) r = m;
                else l = m;
            }

            order[size++] = {folder[r], r};
            folder[r] = ri;
        }
        order.resize(size);
    };

    int d = 0;
    vector<int> folders_rev, offset{0};
    for (int count = 0; count - d * d != n; d++) {
        build(left);
        count += folder.size();

        for (int i = 0; i < folder.size(); i++) cout << ids[folder[i]].first << (i + 1 == folder.size() ? ";" : " ");
        cout << "\n";

        build(right);
        count += folder.size();

        folders_rev.insert(folders_rev.end(), folder.begin(), folder.end());
        offset.emplace_back(folders_rev.size());
    }

    for (int i = d; i < offset[1]; i++) {
        for (int j = 0; j < d && i < offset[j + 1] - offset[j]; j++) cout << ids[folders_rev[offset[j] + i]].first << (j + 1 == d || i >= offset[j + 2] - offset[j + 1] ? ";" : " ");
        cout << "\n";
    }
}