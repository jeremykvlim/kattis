#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int count = 0;
    unordered_map<string, int> dp0, dp1, dp2;
    string w;
    while (cin >> w) {
        int c = 1;

        c = max(c, dp0[w] + 1);

        for (int i = 0; i < w.size(); i++) {
            auto s1 = w.substr(0, i) + w.substr(i + 1), s2 = w;
            s2[i] = '#';
            c = max({c, dp1[s1] + 1, dp2[s2] + 1});
        }

        dp1[w] = c;
        count = max(count, c);

        for (int i = 0; i < w.size(); i++) {
            auto s1 = w.substr(0, i) + w.substr(i + 1), s2 = w;
            s2[i] = '#';
            dp0[s1] = max(dp0[s1], c);
            dp2[s2] = max(dp2[s2], c);
        }
    }
    cout << count;
}
