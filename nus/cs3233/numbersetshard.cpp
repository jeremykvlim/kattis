#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
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
