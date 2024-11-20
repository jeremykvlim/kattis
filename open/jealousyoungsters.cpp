#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    if (m < n) {
        cout << "impossible";
        exit(0);
    }

    int d, e;
    cin >> d >> e;

    vector<vector<int>> start(n, vector<int>(m, -1)), first(n, vector<int>(m, -1)), total(n, vector<int>(m, 0)), desirable(n, vector<int>(m, 0));
    vector<int> pos(n, 0), toy(n, 0);
    while (e--) {
        int s, k, t;
        cin >> s >> k >> t;
        k--;

        if (~start[k][toy[k]]) {
            total[k][toy[k]] += s - start[k][toy[k]];
            start[k][toy[k]] = -1;
        }

        if (!t || t > n) continue;
        t--;

        toy[k] = t;
        start[k][t] = s;

        if (!~first[k][t]) {
            first[k][t] = s;
            desirable[k][pos[k]++] = t;
        }
    }

    for (int k = 0; k < n; k++)
        for (int t = 0; t < m; t++) {
            if (!~first[k][t]) desirable[k][pos[k]++] = t;
            if (~start[k][t]) total[k][t] += d - start[k][t];
        }

    vector<int> kid(m, -1);
    fill(pos.begin(), pos.end(), 0);
    fill(toy.begin(), toy.end(), 0);
    for (int i = 0; i < n; i++) {
        int k = i;
        while (~k) {
            int t = desirable[k][pos[k]];
            if (!~kid[t] || total[k][t] < total[kid[t]][t]) {
                int temp = kid[t];
                kid[t] = k;
                toy[k] = t + 1;
                k = temp;
            } else pos[k]++;
        }
    }
    for (int t : toy) cout << t << " ";
}
