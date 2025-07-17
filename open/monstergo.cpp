#include <bits/stdc++.h>
using namespace std;

int C(int n, int k) {
    return !k ? 1 : C(n - 1, k - 1) * n / k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<array<int, 3>>> chunks(51, vector<array<int, 3>>(N + 1, {0, 0, 0}));
    auto dp = [&](auto &&self, int f, int m = 50) -> bool {
        if (!f) return true;
        if (!m) return false;
        if (self(self, f, m - 1)) return true;

        for (int d : {1, 2, 3, 4, 6, 12}) {
            int k = 12 / d;
            for (int n = k; n * d <= m && n <= 13; n++) {
                int ways = C(n, k);
                if (f >= ways && self(self, f - ways, m - n * d)) {
                    chunks[m][f] = {d, n, ways};
                    return true;
                }
            }
        }

        return false;
    };
    dp(dp, N);

    vector<array<int, 3>> path;
    for (int m = 50, f = N; f;) {
        auto [d, n, ways] = chunks[m][f];
        if (!d) m--;
        else {
            path.push_back({d, n, ways});
            m -= d * n;
            f -= ways;
        }
    }
    reverse(path.begin(), path.end());

    vector<int> types(50);
    iota(types.begin(), types.end(), 0);
    for (auto [d, n, ways] : path) {
        vector<int> used(types.begin(), types.begin() + d * n);
        types.erase(types.begin(), types.begin() + d * n);

        int k = 12 / d;
        vector<int> indices(k);
        iota(indices.begin(), indices.end(), 0);
        for (;;) {
            for (int i : indices)
                for (int j = 0; j < d; ++j) cout << used[i * d + j] << " ";
            cout << "\n";
            if (!--N) exit(0);

            int i = k - 1;
            while (i >= 0 && indices[i] == n - k + i) i--;
            if (i < 0) break;

            indices[i]++;
            for (int j = i + 1; j < k; j++) indices[j] = indices[j - 1] + 1;
        }
    }
}
