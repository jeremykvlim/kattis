#include <bits/stdc++.h>
using namespace std;

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
    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    int biggest = 0;
    for (int &ai : a) {
        cin >> ai;

        biggest = max(biggest, ai);
    }

    vector<int> spf = sieve(biggest), prev(biggest + 1, -1), left(n, -1), right(n, n), parent(n, 0);
    for (int i = 0; i < n; i++)
        for (int ai = a[i]; ai > 1; ai /= spf[ai]) {
            if (prev[spf[ai]] < i) {
                left[i] = max(left[i], prev[spf[ai]]);
                if (prev[spf[ai]] != -1) right[prev[spf[ai]]] = min(right[prev[spf[ai]]], i);
            }
            prev[spf[ai]] = i;
        }

    auto dfs = [&](auto &&self, int l, int r, int p = -1) {
        if (l + 1 > r) return true;
        for (int i = l, j = r - 1; i <= j; i++, j--) {
            parent[i] = p;
            if (left[i] < l && right[i] >= r) return self(self, l, i, i) && self(self, i + 1, r, i);
            parent[j] = p;
            if (left[j] < l && right[j] >= r) return self(self, l, j, j) && self(self, j + 1, r, j);
        }

        return false;
    };

    if (dfs(dfs, 0, n))
        for (int p : parent) cout << p + 1 << " ";
    else cout << "impossible";
}
