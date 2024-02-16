#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 10567201;

pair<long long, long long> bezout(int a, int b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, v;
    while (cin >> n >> m >> v && n && m) {
        auto p2 = 1LL;
        while (n--) p2 = (p2 << 1) % MODULO;

        vector<long long> C(m + 1, 1), ways(m + 1, 1);
        for (int k = 1; k <= m; k++) {
            auto [x, y] = bezout(k, MODULO);
            if (x < 0) x += MODULO;
            C[k] = ((__int128) C[k - 1] * (p2 + 1 - k) * x) % MODULO;
        }

        if (v) ways[0] = 0;
        for (int k = 2; k <= m; k++) {
            auto temp = ((__int128) ways[k - 2] * (p2 + 2 - k + MODULO)) % MODULO;
            temp = (C[k - 1] - temp + MODULO) % MODULO;

            auto [x, y] = bezout(k, MODULO);
            if (x < 0) x += MODULO;
            ways[k] = (temp * x) % MODULO;
        }

        cout << ways[m] << "\n";
    }
}
