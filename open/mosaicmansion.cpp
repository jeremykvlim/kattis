#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, c;
    cin >> n >> m >> c;

    mt19937_64 rng(random_device{}());
    vector<unsigned long long> hash(c);
    for (int i = 1; i < c; i++) {
        hash[i] = rng();
        hash[0] -= hash[i];
    }

    vector<unsigned long long> row_hash(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            int p;
            cin >> p;

            row_hash[i] += hash[p - 1];
        }

    int k = n / 2, l = 1 << k, r = 1 << (n - k);
    unordered_map<unsigned long long, int> memo;
    vector<unsigned long long> sum(l, 0);
    for (int i = 0; i < l; i++) {
        if (i) sum[i] = sum[i & (i - 1)] + row_hash[countr_zero((unsigned) i)];

        int p = popcount((unsigned) i);
        auto [it, inserted] = memo.emplace(sum[i], p);
        if (!inserted) it->second = max(it->second, p);
    }

    int keep = 0;
    sum.assign(r, 0);
    for (int i = 0; i < r; i++) {
        if (i) sum[i] = sum[i & (i - 1)] + row_hash[k + countr_zero((unsigned) i)];

        auto it = memo.find(-sum[i]);
        if (it != memo.end()) keep = max(keep, popcount((unsigned) i) + it->second);
    }
    cout << keep;
}
