#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    auto wrap = [&](__int128 mask, int s) {
        return ((mask >> (m - s)) ^ (mask << s)) & (((__int128) 1 << m) - 1);
    };

    int n = 1 << 19;
    long long x1 = 0, y1 = 1, x2 = 1, y2 = 0;
    vector<pair<long long, int>> baby(n), giant(n);
    for (int i = 0; i < n; i++) {
        auto mask = (y1 << m) | x1;
        baby[i] = {mask, i};
        x1 ^= wrap(y1, 1);
        swap(x1, y1);
    }

    for (int i = 0, k = n % m; i < n; i++) {
        auto mask = (y2 << m) | x2;
        giant[i] = {mask, i};
        x2 ^= wrap(y1, k);
        y2 ^= wrap(x1, k);
        swap(y1, x2);
        swap(x1, y2);
    }

    auto dedupe = [&](auto &v) {
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end(), [&](auto p1, auto p2) { return p1.first == p2.first; }), v.end());
    };
    dedupe(baby);
    dedupe(giant);

    auto rounds = LLONG_MAX;
    for (int i = 0, j = 0; i < baby.size() && j < giant.size();) {
        if (baby[i].first == giant[j].first) {
            rounds = min(rounds, ((long long) giant[j].second << 19) + baby[i].second);
            i++;
            j++;
        } else if (baby[i].first < giant[j].first) i++;
        else j++;
    }
    cout << rounds + 1;
}
