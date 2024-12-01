#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_set<string> dict;
    while (n--) {
        string s;
        cin >> s;

        sort(s.begin(), s.end());
        dict.emplace(s);
    }

    int m;
    cin >> m;

    while (m--) {
        int p;
        cin >> p;

        vector<pair<char, int>> tiles(p);
        for (auto &[c, v] : tiles) cin >> c >> v;
        sort(tiles.begin(), tiles.end());

        int score = 0;
        for (int mask = 0; mask < (1 << p); mask++) {
            string s;
            int sum = 0;
            for (int i = 0; i < p; i++) {
                if (mask & (1 << i)) {
                    auto [c, v] = tiles[i];
                    s += c;
                    sum += v;
                }
            }
            if (dict.count(s)) score = max(score, sum);
        }
        cout << score << "\n";
    }
}
