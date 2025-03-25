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
    vector<int> factors(n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<int> primes = sieve(1e6);
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
