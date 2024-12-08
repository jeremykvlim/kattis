#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
U hungarian(const vector<vector<T>> &adj_matrix, const U delta) {
    int R = adj_matrix.size(), C = adj_matrix[0].size();

    vector<int> r_match(R, -1), c_match(C, -1);
    vector<T> potential(R, 0);
    for (int c = 0; c < C; c++) {
        int r = 0;
        for (int row = 1; row < R; row++)
            if (adj_matrix[r][c] > adj_matrix[row][c]) r = row;

        potential[c] = adj_matrix[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return adj_matrix[r][c] - potential[c];
    };

    vector<int> cols(C);
    iota(cols.begin(), cols.end(), 0);
    for (int r = 0; r < R; r++) {
        if (r_match[r] != -1) continue;

        vector<T> dist(C);
        for (int c = 0; c < C; c++) dist[c] = diff(r, c);

        vector<int> prev(C, r);
        int scanned = 0, labeled = 0, last = 0, curr;
        for (;;) {
            if (scanned == labeled) {
                auto d = dist[cols[scanned]];
                for (int c = scanned; c < C; c++)
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
            for (int c = labeled; c < C; c++) {
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
    for (int r = 0; r < R; r++) cost += adj_matrix[r][r_match[r]];
    return cost - delta;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long a, b, c;
    cin >> n >> a >> b >> c;

    vector<long long> x(n + 1, 0);
    for (int i = 1; i <= n; i++) cin >> x[i];

    auto time = [&](auto &&self, long long x, long long y) -> long long {
        if (x == y) return 0;

        auto t = x ? self(self, 0, y) + c : LLONG_MAX;
        if (x > y) return t;

        if (LLONG_MAX >= (__int128) a * (y - x)) t = min(t, a * (y - x));
        if (x <= y / 2) t = min(t, self(self, x, y / 2) + (y & 1 ? a : 0) + (y > 1 ? b : 0));

        return t;
    };

    vector<vector<long long>> adj_matrix(n + 1, vector<long long>(n + 1));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            adj_matrix[i][j] = i == j ? LLONG_MAX : time(time, x[i], x[j]);

    cout << hungarian(adj_matrix, c);
}
