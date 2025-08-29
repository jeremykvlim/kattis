#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r;
    cin >> r;

    int total = 0;
    vector<string> rules(r);
    for (auto &s : rules) {
        cin >> s;

        total += pow(10, count_if(s.begin(), s.end(), [&](auto c) { return c == '*'; }));
    }

    cout << total << "\n";
    for (auto &s : rules) {
        auto t = s;
        vector<int> indices;
        for (int i = 0; i < t.size(); i++)
            if (t[i] == '*') indices.emplace_back(i);

        auto dfs = [&](auto &&self, int i = 0) -> void {
            if (i == indices.size()) {
                cout << t << "\n";
                return;
            }
            
            for (int d = 0; d <= 9; d++) {
                t[indices[i]] = '0' + d;
                self(self, i + 1);
            }
        };
        dfs(dfs);
    }
}
