#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    auto index = [](char c) {
        switch (c) {
            case 'A': return 0;
            case 'C': return 1;
            case 'G': return 2;
            case 'T': return 3;
        }
    };

    vector<string> dna(n);
    mt19937_64 rng(random_device{}());
    vector<long long> r(n);
    vector<vector<long long>> sum(m, vector<long long>(4, 0));
    auto total = 0LL;
    for (int i = 0; i < n; i++) {
        cin >> dna[i];

        r[i] = rng();
        for (int j = 0; j < m; j++) sum[j][index(dna[i][j])] += r[i];
        total += r[i];
    }

    for (int i = 0; i < n; i++) {
        auto diff = 0LL;
        for (int j = 0; j < m; j++) diff += total - sum[j][index(dna[i][j])];
        if (diff == k * (total - r[i])) {
            cout << i + 1;
            exit(0);
        }
    }
}
