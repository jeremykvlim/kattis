#include <bits/stdc++.h>
using namespace std;

struct Program {
    int repeats, index;
    vector<Program> loop;

    Program(int r = 0) : repeats(r), index(-1) {}
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
                Program child;
                child.index = i - 1;
                root.loop.emplace_back(child);
            } else {
                int n = S[++l] - '0';
                while (S[++l] != ' ') n = n * 10 + S[l] - '0';

                int temp = l++;
                for (int program = 1; program; l++) program += (S[l] == 'R') - (S[l] == 'E');

                auto child = self(self, temp, l - 1);
                child.repeats = n;
                root.loop.emplace_back(move(child));
            }
        }

        return move(root);
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
    int size = 1 << v, banks = min(b - 1, v / (s / 2 + 1)) - 1;
    vector<long long> dp1(size), dp2(size);
    for (int mask = 0; mask < size; mask++)
        if (__builtin_popcount(mask) == s) {
            for (auto &row : count) fill(row.begin(), row.end(), 0);
            R = C = -1;
            run(run, root, mask, 1);

            auto curr = base + (C != -1);
            for (auto row : count) curr += accumulate(row.begin(), row.end(), 0LL);

            if (s == 1) {
                for (int i = 0; i < v; i++) curr -= count[i][i];
                least = min(least, curr);
                continue;
            }

            int m = size + ~mask;
            for (int k = (mask + 1) & m; k; k = ((k | mask) + 1) & m) {
                dp1[k] = 0;

                if (__builtin_popcount(k) <= s) {
                    int i = __builtin_ctz(k);
                    dp2[k] = dp2[k & (k - 1)] - count[i][i];
                    for (int j = i + 1; j < v; j++)
                        if (k & (1 << j)) dp2[k] -= count[i][j] + count[j][i];

                    dp1[k] = dp2[k];
                }
            }

            for (int h = banks, k = 1 << __lg(m); h && k && m - k; h--, k |= 1 << __lg(m - k))
                for (int i = m; i > 0; --i &= m)
                    for (int j = i - k; j > 0; --j &= i - k)
                        dp1[i] = min(dp1[i], dp1[i - j] + dp2[j]);

            least = min(least, curr + dp1[m]);
        }

    cout << least;
}
