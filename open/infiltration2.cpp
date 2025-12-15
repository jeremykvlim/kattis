#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int n, t = 1; cin >> n; t++) {
        vector<__int128> cells(n, 0);
        for (int i = 0; i < n; i++) {
            string s;
            cin >> s;

            for (int j = 0; j < n; j++)
                if (s[j] == '1') cells[i] |= (__int128) 1 << j;
            cells[i] |= (__int128) 1 << i;
        }

        vector<int> c;
        auto m1 = ((__int128) 1 << n) - 1;
        auto dfs = [&](auto &&self, int m, int i = 0) -> bool {
            if (!m) {
                __int128 m2 = 0;
                for (int ci : c) m2 |= cells[ci];
                return !(m1 & ~m2);
            }

            if (m < n - i)
                if (self(self, m, i + 1)) return true;

            c.emplace_back(i);
            if (self(self, m - 1, i)) return true;
            c.pop_back();
            return false;
        };

        int m = 1;
        for (; !dfs(dfs, m); m++) c.clear();

        cout << "Case " << t << ": " << m << " ";
        for (int ci : c) cout << ci + 1 << " ";
        cout << "\n";
    }
}
