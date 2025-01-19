#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> fib(87);
    fib[0] = fib[1] = 1;
    for (int i = 2; i < 87; i++) fib[i] = fib[i - 1] + fib[i - 2];

    vector<array<int, 3>> bash, chipaku;
    for (int i = 0; i < 60; i++) {
        bash.push_back({i + 3, 2, i + 1});
        for (int j = 0; j < i; j++) bash.push_back({i + 3, j + 3, i - j});
    }

    for (int i = 3; i < 89; i++) {
        chipaku.push_back({i, i - 1, 1});
        if (i > 3) chipaku.push_back({i, i - 2, 2});
    }

    long long a;
    while (cin >> a && ~a) {
        auto print = [&](int n, const auto &edges) {
            cout << "YES\n\n" << n << " " << edges.size() << "\n";
            for (auto [u, v, c] : edges) cout << u << " " << v << " " << c << "\n";
        };

        auto b = bash, c = chipaku;
        for (int i = 0; i < 60; i++)
            if ((a >> i) & 1) b.push_back({1, i + 3, 60 - i});

        for (int i = 86; ~i; i--)
            if (a >= fib[i]) {
                a -= fib[i];
                c.push_back({1, i + 2, 86 - i + 1});
            }

        print(62, b);
        print(88, c);
    }
}
