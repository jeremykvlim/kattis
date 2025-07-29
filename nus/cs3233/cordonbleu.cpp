#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
U hungarian(const vector<vector<T>> &C, const U delta) {
    int n = C.size();

    vector<int> r_match(n, -1), c_match(n, -1);
    vector<T> potential(n, 0);
    for (int c = 0; c < n; c++) {
        int r = 0;
        for (int row = 1; row < n; row++)
            if (C[r][c] > C[row][c]) r = row;

        potential[c] = C[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return C[r][c] - potential[c];
    };

    vector<int> cols(n);
    iota(cols.begin(), cols.end(), 0);
    for (int r = 0; r < n; r++) {
        if (r_match[r] != -1) continue;

        vector<T> dist(n);
        for (int c = 0; c < n; c++) dist[c] = diff(r, c);

        vector<int> prev(n, r);
        int scanned = 0, labeled = 0, last = 0, curr;
        for (;;) {
            if (scanned == labeled) {
                auto d = dist[cols[scanned]];
                for (int c = scanned; c < n; c++)
                    if (dist[cols[c]] <= d) {
                        if (dist[cols[c]] < d) {
                            d = dist[cols[c]];
                            labeled = scanned;
                        }

                        swap(cols[c], cols[labeled++]);
                    }

                for (int c = scanned; c < labeled; c++)
                    if (c_match[cols[c]] == -1) {
                        curr = cols[c];
                        goto done;
                    }

                last = scanned;
            }

            int c1 = cols[scanned++], r1 = c_match[c1];
            for (int c = labeled; c < n; c++) {
                int c2 = cols[c];
                auto d = diff(r1, c2) - diff(r1, c1);

                if (dist[c2] > dist[c1] + d) {
                    dist[c2] = dist[c1] + d;
                    prev[c2] = r1;

                    if (!d) {
                        if (c_match[c2] == -1) {
                            curr = c2;
                            goto done;
                        }

                        swap(cols[c], cols[labeled++]);
                    }
                }
            }
        }

        done:;
        for (int c = 0; c < last; c++) potential[cols[c]] += dist[cols[c]] - dist[curr];
        int temp = curr;
        while (temp != -1) {
            curr = temp;
            c_match[curr] = prev[curr];
            swap(r_match[prev[curr]], temp);
        }
    }

    U cost = 0;
    for (int r = 0; r < n; r++) cost += C[r][r_match[r]];
    return cost - delta;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> b(n), c(m);
    pair<int, int> r;
    for (auto &[x, y] : b) cin >> x >> y;
    for (auto &[x, y] : c) cin >> x >> y;
    cin >> r.first >> r.second;

    auto diff = [&](pair<int, int> x, pair<int, int> y) {
        return abs(x.first - y.first) + abs(x.second - y.second);
    };

    vector<vector<int>> C(n + m - 1, vector<int>(n + m - 1, 1e6));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) C[i][j] = min(C[i][j], diff(b[i], c[j]));
        for (int j = m; j < n + m - 1; j++) C[i][j] = min(C[i][j], diff(b[i], r));
    }

    auto cost = hungarian(C, (int) 1e6 * (m - 1));
    for (auto bi : b) cost += diff(bi, r);
    cout << cost;
}
