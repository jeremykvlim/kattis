#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> count(26, vector<int>(26, 0));
    while (n--) {
        string s;
        cin >> s;

        count[s[0] - 'a'][s.back() - 'a']++;
    }

    auto win = [&](auto count, int v) {
        for (int i = 0; i < 26; i++) {
            count[i][i] &= 1;
            for (int j = 0; j < i; j++) {
                int temp = min(count[i][j], count[j][i]);
                count[i][j] -= temp;
                count[j][i] -= temp;
            }
        }

        auto dfs = [&](auto &&self, int v) -> bool {
            for (int u = 0; u < 26; u++)
                if (count[v][u]) {
                    count[v][u]--;
                    bool b = self(self, u);
                    count[v][u]++;
                    if (b) return false;
                }
            return true;
        };
        return dfs(dfs, v);
    };

    int words = 0;
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            if (count[i][j]) {
                count[i][j]--;
                if (win(count, j)) words += count[i][j] + 1;
                count[i][j]++;
            }
    cout << words;
}
