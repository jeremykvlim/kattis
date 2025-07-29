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

    vector<vector<long long>> C(n + 1, vector<long long>(n + 1, LLONG_MAX));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            if (i != j) C[i][j] = time(time, x[i], x[j]);

    cout << hungarian(C, c);
}
