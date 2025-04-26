#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> towers(1);
    vector<int> x_indices(n, 0), height(1, 0), height_indices(1, 0);
    while (m--) {
        char c;
        int i, j;
        cin >> c >> i >> j;
        i--;
        j--;

        if (c == '?') {
            if (height[x_indices[i]] > height[x_indices[j]]) cout << ">\n";
            else if (height[x_indices[i]] == height[x_indices[j]]) cout << "=\n";
            else cout << "<\n";
        } else {
            auto t = towers[x_indices[i]];
            t.emplace_back(x_indices[j]);
            sort(t.rbegin(), t.rend(), [&](int i, int j) { return height[i] < height[j]; });

            int l = 0, r = height.size(), mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                auto cmp = [&](const auto &t1, const auto &t2) {
                    for (int i = 0; i < min(t1.size(), t2.size()); i++)
                        if (t1[i] != t2[i]) return height[t1[i]] < height[t2[i]];
                    return t1.size() <= t2.size();
                };

                if (cmp(towers[height_indices[mid]], t)) l = mid;
                else r = mid;
            }

            if (towers[height_indices[l]] == t) x_indices[i] = height_indices[l];
            else {
                towers.emplace_back(t);
                x_indices[i] = height.size();
                for (int &h : height)
                    if (h >= r) h++;
                height.emplace_back(r);
                height_indices.emplace_back();
                for (int k = 0; k < height.size(); k++) height_indices[height[k]] = k;
            }
        }
    }
}
