#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << 1;
        exit(0);
    }

    int mask = 0;
    vector<int> s(n);
    for (int &si : s) {
        cin >> si;
        si--;

        mask |= si;
    }

    unordered_set<int> seen;
    auto dfs = [&](auto &&self, int b = 31) -> int {
        if (b < 0) return 0;
        if (!((mask >> b) & 1)) return self(self, b - 1);

        mask ^= 1 << b;
        seen.clear();
        for (int si : s) {
            si &= mask;

            if (seen.count(si)) seen.erase(si);
            else seen.emplace(si);
        }

        if (!seen.empty()) return self(self, b - 1);

        vector<int> temp;
        for (int si : s)
            if ((si >> b) & 1) temp.emplace_back(si);
        s = temp;
        return self(self, b - 1) | (1 << b);
    };
    cout << dfs(dfs) + 1;
}
