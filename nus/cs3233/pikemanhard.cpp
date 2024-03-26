#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, a, b, c, t0;
    long long t;
    cin >> n >> t >> a >> b >> c >> t0;

    vector<pair<int, int>> problems;
    for (int i = 0; i < min(n, c); i++) {
        problems.emplace_back(t0, 1);
        t0 = ((long long) a * t0 + b) % c + 1;
    }

    n -= min(n, c);
    a %= c;
    b = (b + 1) % c;

    if (a == 1) {
        for (int i = 0; i < c; i++) {
            auto ti = (t0 + (long long) i * b) % c;
            if (!ti) ti += c;
            problems.emplace_back(ti, n / c + (i < n % c));
        }
    } else {
        int len = 0;
        for (int i = 1; i <= c; i++)
            if (__gcd(i, c) == 1) len++;

        for (int i = 0, ai = 1, sum = 0; i < len; i++) {
            auto ti = ((long long) ai * t0 + (long long) b * sum) % c;
            if (!ti) ti += c;
            problems.emplace_back(ti, n / len + (i < n % len));

            sum = (sum + ai) % c;
            ai = ((long long) ai * a) % c;
        }
    }

    sort(problems.begin(), problems.end());
    long long solved = 0, penalty = 0, elapsed = 0;
    for (auto [ti, count]: problems) {
        auto solvable = min(t / ti, (long long) count);
        t -= ti * solvable;
        if (t < 0) break;
        
        solved += solvable;
        penalty += (elapsed * solvable + (solvable * (solvable + 1) / 2) % MODULO * ti) % MODULO;
        elapsed = (elapsed + ti * solvable) % MODULO;
    }

    cout << solved << " " << penalty % MODULO;
}
