#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string mapping = "22233344455566677778889999";

    int n;
    cin >> n;

    vector<pair<string, string>> dictionary(n);
    unordered_map<string, int> count;
    for (auto &[word, digits] : dictionary) {
        cin >> word;

        for (char c : word) digits += mapping[c - 'a'];
        count[digits]++;
    }

    unordered_map<string, int> seen, move;
    for (auto &[word, digits] : dictionary) {
        int c = count[digits], x = seen[digits]++;
        if (2 * x > c) x -= c;
        auto it = move.find(word);
        if (it == move.end() || abs(x) < abs(it->second)) move[word] = x;
    }

    int q;
    cin >> q;

    vector<int> X(500000), next(500000);
    vector<long long> dp(500001);
    while (q--) {
        string w;
        cin >> w;

        int m = w.size();
        fill(dp.begin(), dp.begin() + m + 1, 1e18);
        dp[m] = 0;
        for (int i = m - 1; ~i; i--) {
            string s;
            for (int j = i; j < m && j < i + 10; j++) {
                s += w[j];
                auto it = move.find(s);
                if (it == move.end()) continue;

                int x = it->second;
                if (dp[i] > dp[j + 1] + j - i + 1 + abs(x) + (j + 1 < m)) {
                    dp[i] = dp[j + 1] + j - i + 1 + abs(x) + (j + 1 < m);
                    X[i] = x;
                    next[i] = j + 1;
                }
            }
        }

        for (int i = 0; i < m;) {
            if (i) cout << 'R';
            for (int j = i; j < next[i]; j++) cout << mapping[w[j] - 'a'];
            if (X[i] > 0) cout << "U(" << X[i] << ")";
            else if (X[i] < 0) cout << "D(" << -X[i] << ")";
            i = next[i];
        }
        cout << "\n";
    }
}