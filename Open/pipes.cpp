#include <bits/stdc++.h>
using namespace std;

struct Hash {
    uint64_t encode(const pair<int, int> &p) const {
        uint64_t encoded = 0;
        encoded = (encoded << 8) | p.first;
        encoded = (encoded << 8) | p.second;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        uint64_t hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const pair<int, int> &p) const {
        static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(p) + SEED);
    }
};

pair<int, int> tile(int l, int m, int n) {
    if (l <= m) return {l - 1, 0};
    if (l <= m + n) return {m - 1, l - m - 1};
    if (l <= 2 * m + n) return {2 * m + n - l, n - 1};
    return {0, 2 * (m + n) - l};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    while (k--) {
        int m, n, l;
        cin >> m >> n >> l;

        vector<int> indices(2 * (m + n) + 1, -1);
        deque<int> locations(2 * l);
        vector<pair<int, int>> connect(l);
        for (int i = 0; i < l; i++) {
            int s, t;
            cin >> s >> t;

            connect[i] = {s, t};
            locations[2 * i] = s;
            locations[2 * i + 1] = t;
            indices[s] = indices[t] = i;
        }
        sort(locations.begin(), locations.end());

        vector<int> order;
        while (!locations.empty()) {
            if (indices[locations[0]] == indices[locations.back()]) {
                order.emplace_back(indices[locations[0]]);
                locations.pop_front();
                locations.pop_back();
                continue;
            }

            bool change = false;
            for (int i = 1; i < locations.size(); i++)
                if (indices[locations[i - 1]] == indices[locations[i]]) {
                    order.emplace_back(indices[locations[i]]);
                    locations.erase(locations.begin() + i - 1);
                    locations.erase(locations.begin() + i - 1);
                    change = true;
                    break;
                }

            if (!change) break;
        }

        if (!locations.empty()) {
            cout << "NO\n";
            next:;
            continue;
        }

        vector<vector<bool>> visited(m, vector<bool>(n, false));
        for (int i : order) {
            auto [r1, c1] = tile(connect[i].first, m, n);
            auto [r2, c2] = tile(connect[i].second, m, n);

            if (visited[r1][c1] || visited[r2][c2]) {
                cout << "NO\n";
                goto next;
            }

            visited[r1][c1] = visited[r2][c2] = true;
        }

        vector<vector<bool>> border(m, vector<bool>(n, false));
        for (int i = 0; i < m; i++) border[i][0] = border[i][n - 1] = true;
        for (int i = 0; i < n; i++) border[0][i] = border[m - 1][i] = true;

        auto out = [&](auto next) -> bool {return next.first < 0 || next.first >= m || next.second < 0 || next.second >= n;};
        vector<int> dr1{1, -1, 0, 0}, dc1{0, 0, 1, -1}, dr2{1, 1, 1, 0, 0, -1, -1, -1}, dc2{1, 0, -1, 1, -1, 1, 0, -1};
        for (int i : order) {
            auto src = tile(connect[i].first, m, n), dest = tile(connect[i].second, m, n);

            unordered_map<pair<int, int>, int, Hash> dist;
            dist[src] = 0;
            unordered_map<pair<int, int>, pair<int, int>, Hash> prev;
            stack<pair<int, int>> s;
            s.emplace(src);
            while (!s.empty()) {
                auto curr = s.top();
                s.pop();

                if (curr == dest) break;

                for (int j = 0; j < 4; j++) {
                    pair<int, int> next{curr.first + dr1[j], curr.second + dc1[j]};

                    if (next == dest) {
                        dist[next] = dist[curr] + 1;
                        prev[next] = curr;
                        goto done;
                    }

                    if (out(next) || visited[next.first][next.second] || !border[next.first][next.second]) continue;

                    if (!dist.count(next)) {
                        dist[next] = dist[curr] + 1;
                        prev[next] = curr;
                        s.emplace(next);
                    }
                }
            }
            done:;

            if (!dist.count(dest)) {
                cout << "NO\n";
                goto next;
            }

            for (int j = 0; j < 8; j++) {
                pair<int, int> next{dest.first + dr2[j], dest.second + dc2[j]};
                if (!out(next)) border[next.first][next.second] = true;
            }

            while (src != dest) {
                dest = prev[dest];
                visited[dest.first][dest.second] = true;

                for (int j = 0; j < 8; j++) {
                    pair<int, int> next{dest.first + dr2[j], dest.second + dc2[j]};
                    if (!out(next)) border[next.first][next.second] = true;
                }
            }
        }

        cout << "YES\n";
    }
}
