#include <bits/stdc++.h>
using namespace std;

struct Program {
    int repeats, index;
    vector<Program> loop;

    Program(int r = 0, int i = -1) : repeats(r), index(i) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, s;
    cin >> b >> s;
    cin.ignore();

    string S;
    getline(cin, S);
    S += ' ';

    int v = 0;
    auto parse = [&](auto &&self, int l, int r) -> Program {
        Program root(1);
        while (l < r) {
            if (S[l] == ' ') l++;
            else if (S[l] == 'V') {
                int i = S[++l] - '0';
                if (S[++l] != ' ') i = i * 10 + S[l++] - '0';

                v = max(v, i);
                Program child(0, i - 1);
                root.loop.emplace_back(child);
            } else {
                int n = S[++l] - '0';
                while (S[++l] != ' ') n = n * 10 + S[l] - '0';

                int temp = l++;
                for (int program = 1; program; l++) program += (S[l] == 'R') - (S[l] == 'E');

                auto child = self(self, temp, l - 1);
                child.repeats = n;
                root.loop.emplace_back(child);
            }
        }
        return root;
    };
    auto root = parse(parse, 0, S.size());

    vector<vector<long long>> count(v, vector<long long>(v, 0));
    int R = -1, C = -1;
    auto run = [&](auto &&self, Program &curr, int mask = 0, long long reps = 1) {
        if (curr.loop.empty()) {
            if ((mask >> curr.index) & 1) C = -1;
            else R = C = curr.index;
            return;
        }

        int r = -1, c = -1;
        for (auto &next : curr.loop) {
            self(self, next, mask, reps * curr.repeats);
            if (~C && ~r) count[r][C] += reps * curr.repeats;
            if (~C) r = R;
            if (!~c) c = C;
        }
        C = c;
        if (~c) count[R][c] += reps * (curr.repeats - 1);
    };
    run(run, root);

    auto base = 1LL;
    for (auto row : count) base += accumulate(row.begin(), row.end(), 0LL);
    if (v <= s) {
        cout << base;
        exit(0);
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
