#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

vector<int> sieve(int n) {
    if (n < 2) return {};

    int r = sqrt(n);
    vector<int> primes{2};
    vector<bool> composite(n / 2 + 1, false);
    for (int o = 3; o <= r; o += 2)
        if (!composite[o / 2]) {
            for (auto m = (long long) o * o; m <= n; m += 2 * o) composite[m / 2] = true;
            primes.emplace_back(o);
        }

    for (int o = (r + 1) | 1; o <= n; o += 2)
        if (!composite[o / 2]) primes.emplace_back(o);
    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    auto primes = sieve(1e6);
    for (int x = 1; x <= t; x++) {
        long long a, b, p;
        cin >> a >> b >> p;

        int size = b - a + 1;
        DisjointSets dsu(size);

        for (int i = primes.size() - 1; ~i && primes[i] >= p; i--) {
            int l = (primes[i] - a % primes[i]) % primes[i];
            for (int r = l + primes[i]; r <= b - a; r += primes[i])
                if (dsu.unite(l, r)) size--;
        }

        cout << "Case #" << x << ": " << size << "\n";
    }
}
