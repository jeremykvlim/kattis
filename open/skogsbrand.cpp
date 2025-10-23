#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    long long t;
    cin >> n >> m >> t;

    vector<long long> xs, ys;
    vector<pair<long long, long long>> burning(n), chopped(m);
    auto xl = LLONG_MAX, xr = LLONG_MIN, yl = LLONG_MAX, yr = LLONG_MIN;
    for (auto &[x, y] : burning) {
        cin >> x >> y;

        xl = min(xl, x);
        xr = max(xr, x);
        yl = min(yl, y);
        yr = max(yr, y);
        xs.emplace_back(x);
        ys.emplace_back(y);
    }

    for (auto &[x, y] : chopped) {
        cin >> x >> y;

        xl = min(xl, x);
        xr = max(xr, x);
        yl = min(yl, y);
        yr = max(yr, y);
        xs.emplace_back(x);
        ys.emplace_back(y);
    }
    xs.emplace_back(xl - t - 1);
    xs.emplace_back(xr + t + 1);
    ys.emplace_back(yl - t - 1);
    ys.emplace_back(yr + t + 1);

    auto compress = [&](auto &coords) {
        sort(coords.begin(), coords.end());
        coords.erase(unique(coords.begin(), coords.end()), coords.end());

        vector<pair<long long, long long>> intervals;
        auto l = coords.front(), r = coords.back();
        for (auto &coord : coords) {
            if (l < coord) intervals.emplace_back(l, coord - 1);
            intervals.emplace_back(coord, coord);
            l = coord + 1;
        }
        if (l <= r) intervals.emplace_back(l, r);
        return intervals;
    };
    auto x_intervals = compress(xs), y_intervals = compress(ys);

    int X = x_intervals.size(), Y = y_intervals.size();
    vector<long long> rows(X), cols(Y);
    for (int i = 0; i < X; i++) {
        auto [l, r] = x_intervals[i];
        rows[i] = r - l + 1;
    }
    for (int i = 0; i < Y; i++) {
        auto [l, r] = y_intervals[i];
        cols[i] = r - l + 1;
    }

    vector<vector<bool>> wall(X, vector<bool>(Y, false));
    auto point = [&](auto tree) -> pair<int, int> {
        auto [x, y] = tree;
        return {lower_bound(x_intervals.begin(), x_intervals.end(), make_pair(x, x)) - x_intervals.begin(), lower_bound(y_intervals.begin(), y_intervals.end(), make_pair(y, y)) - y_intervals.begin()};
    };

    for (auto tree : chopped) {
        auto [x, y] = point(tree);
        wall[x][y] = true;
    }

    vector<vector<vector<long long>>> dist(X, vector<vector<long long>>(Y, vector<long long>(4, 1e18)));
    priority_queue<tuple<long long, int, int, int>, vector<tuple<long long, int, int, int>>, greater<>> pq;
    for (auto tree : burning) {
        auto [x, y] = point(tree);
        if (!wall[x][y])
            for (int z = 0; z < 4; z++) {
                dist[x][y][z] = 0;
                pq.emplace(0, x, y, z);
            }
    }

    auto relax = [&](int x, int y, int z, long long d) {
        if (!(0 <= x && x < X && 0 <= y && y < Y) || wall[x][y] || d > t) return;
        if (dist[x][y][z] > d) {
            dist[x][y][z] = d;
            pq.emplace(d, x, y, z);
        }
    };

    while (!pq.empty()) {
        auto [d, x, y, z] = pq.top();
        pq.pop();

        if (d != dist[x][y][z]) continue;
        if (d > t) break;
        if (wall[x][y]) continue;

        relax(x, y, z ^ 1, d + cols[y] - 1);
        relax(x, y, z ^ 2, d + rows[x] - 1);
        relax(x, y + ((z & 1) ? 1 : -1), z ^ 1, d + 1);
        relax(x + ((z & 2) ? 1 : -1), y, z ^ 2, d + 1);
    }

    auto count = 0LL;
    for (int x = 0; x < X; x++)
        for (int y = 0; y < Y; y++)
            if (!wall[x][y]) {
                auto lu = max(t + 1 - dist[x][y][0], 0LL),
                     ld = max(t + 1 - dist[x][y][2], 0LL),
                     ru = max(t + 1 - dist[x][y][1], 0LL),
                     rd = max(t + 1 - dist[x][y][3], 0LL);
                if (!lu && !ld && !ru && !rd) continue;

                auto r = rows[x], c = cols[y], steps = r + c - 2;
                count += r * c;
                if (r == 1 && c == 1 || lu + rd > steps || ld + ru > steps) continue;

                auto isosceles = [&](auto s) -> long long {
                    if (s <= 1) return 0;
                    return ((s - 1) * (s - 1)) / 4;
                };
                count += isosceles(r - lu - ld) + isosceles(r - rd - ru) + isosceles(c - lu - ru) + isosceles(c - ld - rd);

                auto nwse = steps - lu - rd, nesw = steps - ld - ru;
                count -= (2 * nwse * nesw + 2 * (nwse + nesw) +
                         ((c + lu + ru) & 1) + ((c + ld + rd) & 1) + ((r + lu + ld) & 1) + ((r + rd + ru) & 1)) / 4;
            }
    cout << count;
}
