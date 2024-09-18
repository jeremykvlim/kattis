#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        int k;
        cin >> s >> k;

        int n = s.size();
        vector<vector<vector<int>>> dp1(2, vector<vector<int>>(n, vector<int>(k + 1, -1)));
        vector<vector<int>> dp2(n, vector<int>(3, -1));

        auto convert = [&](auto &&self, int k, int i = 0, bool prev = false) {
            if (k < 0) return INT_MIN;
            if (i > n - 3) return 0;
            if (dp1[prev][i][k] != -1) return dp1[prev][i][k];

            int turned = INT_MIN;

            auto mode = [&](int i, int j) {
                if (dp2[i][j] != -1) return dp2[i][j];

                vector<int> count(27, 0);
                for (int k = 0; k < j + 3; k++) count[s[i + k] - 'a' + 1]++;

                int unique = 0, k = 0;
                for (int c = 1; c <= 26; c++)
                    if (count[c]) {
                        unique++;
                        if (!k || count[k] > count[c]) k = c;
                    }

                if (unique == 2 && count[k] == 1)
                    for (int c = 1; c <= 26; c++)
                        if (count[c] && k != c) return dp2[i][j] = c;

                return dp2[i][j] = 0;
            };

            for (int j = 3; j <= 5 && i + j <= n; j++) {
                int c = mode(i, j - 3);
                if (c && (!prev || (c != s[i - 1] - 'a' + 1 || mode(i - 3, 0) != s[i] - 'a' + 1)))
                    turned = max(turned, self(self, k - 1, i + j, true) + j);
            }

            return dp1[prev][i][k] = max(turned, self(self, k, i + 1));
        };

        cout << convert(convert, k) << "\n";
    }
}
