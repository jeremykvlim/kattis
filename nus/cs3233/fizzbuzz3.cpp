#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> s{"_in_", "inals_stands_for_", "izz_Buzz!"};
    vector<long long> lens{1};
    while (lens.back() <= 1e18) lens.emplace_back(3 * lens.back() + 30);

    auto dfs = [&](auto &&self, int y, long long x) -> char {
        if (x >= lens[y]) return '?';
        if (!y) return 'F';

        for (auto &t : s) {
            if (x < lens[y - 1]) return self(self, y - 1, x);
            x -= lens[y - 1];
            if (x < t.size()) return t[x];
            x -= t.size();
        }
        return '?';
    };

    int q;
    cin >> q;

    while (q--) {
        long long x, y;
        cin >> x >> y;
        cout << dfs(dfs, min(y + 1, (long long) lens.size() - 1), x - 1);
    }
}
