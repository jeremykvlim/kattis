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
        __int128 m1 = ((__int128) 1 << n) - 1;
        auto backtrack = [&](auto &&self, int k, int ci = 0) -> bool {
            if (!k) {
                __int128 m2 = 0;
                for (int ci : c) m2 |= cells[ci];
                return !(m1 & ~m2);
            }

            if (n - ci > k)
                if (self(self, k, ci + 1)) return true;

            c.emplace_back(ci);
            if (self(self, k - 1, ci)) return true;
            c.pop_back();
            return false;
        };

        int m = 1;
        while (!backtrack(backtrack, m)) {
            c.clear();
            m++;
        }
        cout << "Case " << t << ": " << m << " ";
        for (int ci : c) cout << ci + 1 << " ";
        cout << "\n";
    }
}
