#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, c;
        cin >> n >> c;

        vector<long long> a(n);
        unordered_set<long long> seen;
        for (auto &ai : a) {
            cin >> ai;

            seen.emplace(ai);
        }

        mt19937 rng(random_device{}());
        auto len = -1LL;
        for (int fails = 0; fails < 175;) {
            int j = rng() % n, offset;
            do {
                offset = rng() % (2 * c + 1) - c;
            } while (!offset || j + offset < 0 || j + offset >= n);

            auto l = a[j], r = a[j], diff = abs(a[j + offset] - a[j]);
            while (seen.count(l - diff)) l -= diff;
            while (seen.count(r + diff)) r += diff;

            auto curr = (r - l) / diff + 1;
            if (curr > n / c && curr > len) len = curr;
            else fails++;
        }

        cout << len << "\n";
    }
}
