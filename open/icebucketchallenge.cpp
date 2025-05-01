#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, d, rs, cs;
    cin >> r >> c >> d >> rs >> cs;
    rs--;
    cs--;

    int h_max = 0;
    vector<vector<int>> h(r, vector<int>(c));
    for (auto &row : h)
        for (int &hij : row) {
            cin >> hij;

            h_max = max(h_max, hij);
        }

    vector<vector<pair<int, int>>> water(h_max + 1);
    water[h[rs][cs]].emplace_back(rs, cs);
    vector<vector<int>> depth(r, vector<int>(c, 0)), dist(r, vector<int>(c, 0)), visited(r, vector<int>(c, -1)), sources(r, vector<int>(c, -1));
    sources[rs][cs] = h[rs][cs];
    vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
    for (int height = h_max; ~height; height--)
        if (!water[height].empty()) {
            queue<pair<int, int>> q;
            for (auto [ri, ci] : water[height])
                if (visited[ri][ci] != height) {
                    visited[ri][ci] = height;
                    dist[ri][ci] = 0;
                    depth[ri][ci] = d;
                    q.emplace(ri, ci);
                }

            while (!q.empty()) {
                auto [rv, cv] = q.front();
                q.pop();

                if (dist[rv][cv] == d - 1) continue;

                for (int k = 0; k < 4; k++) {
                    int ru = rv + dr[k], cu = cv + dc[k];
                    if (!(0 <= ru && ru < r && 0 <= cu && cu < c) || h[ru][cu] != height || visited[ru][cu] == height) continue;

                    visited[ru][cu] = height;
                    dist[ru][cu] = dist[rv][cv] + 1;
                    depth[ru][cu] = max(depth[ru][cu], d - dist[rv][cv] - 1);
                    q.emplace(ru, cu);
                }

                for (int k = 0; k < 4; k++) {
                    int ru = rv + dr[k], cu = cv + dc[k];
                    if (!(0 <= ru && ru < r && 0 <= cu && cu < c) || h[ru][cu] >= height) continue;

                    if (sources[ru][cu] != h[ru][cu]) {
                        sources[ru][cu] = h[ru][cu];
                        water[h[ru][cu]].emplace_back(ru, cu);
                    }
                }
            }
        }

    for (auto &row : depth) {
        for (int de : row) cout << de << " ";
        cout << "\n";
    }
}
