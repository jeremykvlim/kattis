#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> p(k), d(k);
    for (int &pi : p) cin >> pi;
    for (int i = 0; i < k; i++) {
        cin >> d[i];

        d[i] -= p[i];
    }

    vector<int> count(n, 0);
    for (int i = 0;; i++) {
        int sum = 0;
        auto dfs = [&](auto &&self, int v, int depth = 0) {
            sum += count[depth] = v;

            if (depth == k) return true;

            int u = v + d[depth];
            return u >= 0 && self(self, u, depth + 1);
        };

        if (dfs(dfs, i)) {
            int t = sum;

            for (;;) {
                int c = 0;

                while (p[0] > 2 && count[0]) {
                    p[0]--;
                    count[0]--;
                    c++;
                    sum--;
                }

                for (int j = 1; j < k; j++)
                    while (p[j] - p[j - 1] >= 4 && count[j]) {
                        p[j - 1]++;
                        p[j]--;
                        count[j]--;
                        c++;
                        sum--;
                    }

                while (p[k - 1] <= n - 2 && count[k]) {
                    p[k - 1]++;
                    count[k]--;
                    c++;
                    sum--;
                }

                if (!c) break;
            }

            cout << (!sum ? to_string(t) : "impossible");
            exit(0);
        }
    }
}
