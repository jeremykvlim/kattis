#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string x, y;
    while (cin >> x >> y && x != "0" && y != "0") {
        int len_x = x.size(), len_y = y.size();
        vector<vector<int>> lcs(len_x + 1, vector<int>(len_y + 1, 0));
        for (int i = 0; i < len_x; i++)
            for (int j = 0; j < len_y; j++)
                if (x[i] == y[j]) lcs[i + 1][j + 1] = lcs[i][j] + 1;
                else lcs[i + 1][j + 1] = max(lcs[i][j + 1], lcs[i + 1][j]);

        int l = min(len_x, len_y), len = lcs[len_x][len_y], extra = max(len_x, len_y) - len, n = 0;
        for (; extra; n++) {
            extra -= min(extra, (extra + len) / 2);
            len += min(extra + len, l - len);
        }
        cout << max(1, n) << "\n";
    }
}
