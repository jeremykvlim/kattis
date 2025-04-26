#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<pair<int, int>> tasks(n);
    for (auto &[t, d] : tasks) cin >> t >> d;

    long long l = 1, r = 1, m, total = accumulate(tasks.begin(), tasks.end(), 0LL, [](auto t, auto p) { return t + p.first; }), resets = 0;
    auto complete = [&](auto attempts) {
        auto remaining = attempts * c, required = 0LL;
        int count = 0;
        for (auto [t, d] : tasks) {
            auto spent = min((long long) t / d + (t % d != 0), attempts);
            if (remaining < spent) return false;

            remaining -= spent;
            required += min((long long) t, attempts * d);
            count += spent == attempts;
        }

        if (total > required + min(remaining, (long long) c - count)) return false;
        resets = attempts;
        return true;
    };
    while (!complete(r)) r <<= 1;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (complete(m)) r = m;
        else l = m;
    }

    cout << resets - 1;
}
