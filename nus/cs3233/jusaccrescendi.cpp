#include <bits/stdc++.h>
using namespace std;

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

    int t, B;
    cin >> t >> B;

    int total = t * (t - 1) * (t - 2) / 6;
    if (total < B) {
        cout << "IMPOSSIBLE";
        exit(0);
    }
    total -= B;

    auto primes = sieve(1e3);
    int p = primes.back();
    int el = max(0, t - (p - p / 2)), er = min(t, p / 2), bound = 0;
    for (int e = el; e <= er; e++) {
        int o = t - e;
        bound = max(bound, o * (e / 2) * (e - e / 2) + e * (o / 2) * (o - o / 2));
    }

    if (total > bound) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    int even = 0, u = 0, v = 0;
    for (int e = el; e <= er; e++) {
        int o = t - e;

        unordered_map<int, int> indices;
        for (int i = 0; i <= o; i++)
            if (!indices.count(i * (o - i))) indices[i * (o - i)] = i;

        for (int j = 0; j <= e; j++) {
            int remaining = total - o * j * (e - j);
            if (remaining < 0) continue;
            if (!e) {
                if (!remaining) {
                    u = j;
                    goto done;
                }
                continue;
            }

            if (!(remaining % e) && indices.count(remaining / e)) {
                even = e;
                u = j;
                v = indices[remaining / e];
                goto done;
            }
        }
    }
    cout << "IMPOSSIBLE";
    exit(0);

    done:;
    auto print = [&](int x, bool b) {
        int y = (x * x) % p;
        if (!((y & 1) ^ b)) y -= p;
        cout << x << " " << y << "\n";
    };
    for (int x = 0; x < u; x++) print(2 * x, true);
    for (int x = u; x < even; x++) print(2 * x, false);
    for (int x = 0; x < v; x++) print(2 * x + 1, true);
    for (int x = v; x < t - even; x++) print(2 * x + 1, false);
}
