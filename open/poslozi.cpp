#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> perm(n), shifts(n);
    vector<long long> masks(n);
    vector<vector<int>> dist(n, vector<int>(n, 1e3));
    for (int i = 0; i < n; i++) {
        cin >> perm[i];
        perm[i]--;

        shifts[i] = i << 2;
        masks[i] = 15LL << shifts[i];
        dist[i][i] = 0;
    }

    vector<int> a(m), b(m);
    for (int i = 0; i < m; i++) {
        cin >> a[i] >> b[i];
        a[i]--;
        b[i]--;

        if (a[i] == b[i]) continue;
        dist[a[i]][b[i]] = dist[b[i]][a[i]] = 1;
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    int base = 0;
    auto start = 0LL, finish = 0LL;
    for (int i = 0; i < n; i++) {
        base += dist[i][perm[i]];
        start |= (long long) perm[i] << shifts[i];
        finish |= (long long) i << shifts[i];
    }

    if (start == finish) {
        cout << 0;
        exit(0);
    }

    unordered_map<long long, int> prev;
    vector<deque<array<long long, 3>>> buckets(2e3 + 1);
    buckets[(base + 1) / 2].push_back({start, base, -1});
    for (int f = 0; f <= 2e3; f++) {
        while (!buckets[f].empty()) {
            auto [v, d, j] = buckets[f].front();
            buckets[f].pop_front();

            if (prev.count(v)) continue;
            prev[v] = j;

            if (v == finish) {
                vector<int> p(n);
                iota(p.begin(), p.end(), 0);

                int swaps = 0;
                while (finish != start) {
                    int i = prev[finish];
                    swap(p[a[i]], p[b[i]]);
                    finish = inner_product(p.begin(), p.end(), shifts.begin(), 0LL, bit_or<>(), [](int pi, int shift) { return (long long) pi << shift; });
                    swaps++;
                }
                cout << swaps;
                exit(0);
            }

            int g = f - (d + 1) / 2 + 1;
            for (int i = 0; i < m; i++) {
                int x = (v & masks[a[i]]) >> shifts[a[i]], y = (v & masks[b[i]]) >> shifts[b[i]],
                    D = d - dist[a[i]][x] - dist[b[i]][y] + dist[a[i]][y] + dist[b[i]][x],
                    F = g + (D + 1) / 2;

                if (F < 2e3) {
                    long long z = x ^ y;
                    auto u = v ^ (z << shifts[a[i]]) ^ (z << shifts[b[i]]);

                    if (buckets[F].empty() || u > buckets[F].front()[0]) buckets[F].push_front({u, D, i});
                    else buckets[F].push_back({u, D, i});
                }
            }
        }
    }
}
