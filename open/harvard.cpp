#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, s;
    cin >> b >> s;
    cin.ignore();

    string S;
    getline(cin, S);

    stringstream ss(S);
    vector<string> tokens;
    string t;
    while (ss >> t) tokens.emplace_back(t);

    vector<char> ops;
    vector<int> repeats, indices;
    vector<vector<int>> adj_list;
    int pos = 0, v = 0;
    auto parse = [&](auto &&self) -> int {
        auto node = [&](char op, int n, int i = -1) -> int {
            ops.emplace_back(op);
            repeats.emplace_back(n);
            indices.emplace_back(i);
            adj_list.emplace_back();
            return ops.size() - 1;
        };

        int x = node('#', 1);
        while (pos < tokens.size() && tokens[pos] != "E")
            if (tokens[pos][0] == 'V') {
                int i = stoi(tokens[pos++].substr(1));
                v = max(v, i);
                int y = node('V', 0, i - 1);
                adj_list[x].emplace_back(y);
            } else if (tokens[pos][0] == 'R') {
                int y = node('R', stoi(tokens[pos++].substr(1))), z = self(self);
                adj_list[x].emplace_back(y);
                adj_list[y].emplace_back(z);
            }

        if (pos < tokens.size() && tokens[pos] == "E") pos++;
        return x;
    };
    int root = parse(parse);

    vector<vector<long long>> count(v, vector<long long>(v, 0));
    int R = -1, C = -1;
    auto run = [&](auto &&self, int i, int mask = 0, long long reps = 1) {
        if (ops[i] == 'V') {
            if ((mask >> indices[i]) & 1) C = -1;
            else R = C = indices[i];
            return;
        }
        int r = -1, c = -1;
        for (int j : adj_list[i]) {
            self(self, j, mask, reps * repeats[i]);
            if (~C && ~r) count[r][C] += reps * repeats[i];
            if (~C) r = R;
            if (!~c) c = C;
        }
        C = c;
        if (~c) count[R][c] += reps * (repeats[i] - 1);
    };
    run(run, root);

    auto base = 1LL;
    for (auto row : count) base += accumulate(row.begin(), row.end(), 0LL);
    if (v <= s) {
        cout << base;
        return 0;
    }

    auto least = LLONG_MAX;
    int banks = min(b - 1, v / (s / 2 + 1)) - 1;
    vector<long long> dp1(1 << v, 0), dp2(1 << v, 0);
    for (int m1 = 0; m1 < 1 << v; m1++)
        if (__builtin_popcount(m1) == s) {
            for (auto &row : count) fill(row.begin(), row.end(), 0);
            R = C = -1;
            run(run, root, m1);

            auto curr = base + (C != -1);
            for (auto row : count) curr += accumulate(row.begin(), row.end(), 0LL);

            if (s == 1) {
                for (int i = 0; i < v; i++) curr -= count[i][i];
                least = min(least, curr);
                continue;
            }

            int m2 = ~m1 + (1 << v);
            for (int m3 = (m1 + 1) & m2; m3; m3 = ((m3 | m1) + 1) & m2) {
                dp1[m3] = 0;

                if (__builtin_popcount(m3) <= s) {
                    int i = __builtin_ctz(m3);
                    dp2[m3] = dp2[m3 & (m3 - 1)] - count[i][i];
                    for (int j = i + 1; j < v; j++)
                        if ((m3 >> j) & 1) dp2[m3] -= count[i][j] + count[j][i];

                    dp1[m3] = dp2[m3];
                }
            }

            for (int _ = banks, m3 = 1 << __lg(m2); _ && m3 && m3 != m2; _--, m3 |= 1 << __lg(m2 - m3))
                for (int m4 = m2; m4 > 0; --m4 &= m2)
                    for (int m5 = m4 - m3; m5 > 0; --m5 &= m4 - m3) dp1[m4] = min(dp1[m4], dp1[m4 - m5] + dp2[m5]);
            least = min(least, curr + dp1[m2]);
        }
    cout << least;
}
