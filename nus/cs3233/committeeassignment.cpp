#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m && n || m) {
        unordered_map<string, int> compress;
        vector<int> hostile(n, 0);
        while (m--) {
            string a, b;
            cin >> a >> b;

            int p1 = compress.count(a) ? compress[a] : compress[a] = compress.size(), p2 = compress.count(b) ? compress[b] : compress[b] = compress.size();
            hostile[p1] |= 1 << p2;
            hostile[p2] |= 1 << p1;
        }

        vector<int> members(n, 0);
        auto dfs = [&](auto &&self, int p = 0, int committees = 0) -> int {
            if (n <= committees) return n;

            if (p == hostile.size()) {
                n = min(n, committees);
                return committees;
            }

            int least = hostile.size();
            for (int i = 0; i <= committees; i++)
                if (!(hostile[p] & members[i])) {
                    members[i] |= 1 << p;
                    least = min(least, self(self, p + 1, committees + (i == committees)));
                    members[i] ^= 1 << p;
                }

            return least;
        };
        cout << dfs(dfs) << "\n";
    }
}
