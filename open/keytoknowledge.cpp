#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<string> s(n);
        vector<int> c(n);
        for (int i = 0; i < n; i++) cin >> s[i] >> c[i];

        map<vector<int>, int> count;
        map<vector<int>, string> half;

        int len = m / 2;
        vector<string> possible;
        auto generate = [&](auto &&self, string s = "", int i = 0) {
            if (i == len) {
                possible.emplace_back(s);
                return;
            }

            self(self, s + '0', i + 1);
            self(self, s + '1', i + 1);
        };
        generate(generate);
        
        for (auto p : possible) {
            vector<int> remaining;
            for (int j = 0; j < n; j++) {
                int same = 0;
                for (int k = 0; k < len; k++) same += (p[k] == s[j][k]);
                remaining.emplace_back(c[j] - same);
            }
            count[remaining]++;
            half[remaining] = p;
        }

        auto answers = 0LL;
        string unique;

        len = m - len;
        possible.clear();
        generate(generate);
        
        for (auto p : possible) {
            vector<int> remaining;
            for (int j = 0; j < n; j++) {
                int same = 0;
                for (int k = 0; k < len; k++) same += (p[k] == s[j][k + m / 2]);
                remaining.emplace_back(same);
            }
            answers += count[remaining];
            if (count[remaining]) unique = half[remaining] + p;
        }

        if (answers == 1) cout << unique << "\n";
        else cout << answers << " solutions\n";
    }
}
