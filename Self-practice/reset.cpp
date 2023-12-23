#include <bits/stdc++.h>
using namespace std;

bool complete(vector<pair<int, int>> tasks, long long total, long long &resets, long long attempts, int c) {
    long long remaining = attempts * c;
    for (auto [t, d] : tasks) {
        auto spent = min<long long>(t / d + (t % d != 0), attempts);
        if (remaining < spent) return false;

        remaining -= spent;
        total -= min<long long>(t, attempts * d);
        c -= spent == attempts;
    }

    if (total > min<long long>(remaining, c)) return false;
    resets = attempts;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    long long total = 0;
    vector<pair<int, int>> tasks(n);
    for (auto &t : tasks) {
        cin >> t.first >> t.second;
        total += t.first;
    }

    long long l = 1, r = 1, m, resets = 0;
    while (!complete(tasks, total, resets, r, c)) r <<= 1;
    while (l + 1 < r) {
        m = l + (r - l) / 2;
        if (complete(tasks, total, resets, m, c)) r = m;
        else l = m;
    }

    cout << resets - 1;
}
