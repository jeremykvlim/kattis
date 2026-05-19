#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> masks;
    for (int c1 = 0; c1 < 21; c1++)
        for (int c2 = c1 + 1; c2 < 22; c2++)
            for (int c3 = c2 + 1; c3 < 23; c3++)
                for (int c4 = c3 + 1; c4 < 24; c4++)
                    for (int c5 = c4 + 1; c5 < 25; c5++)
                        for (int c6 = c5 + 1; c6 < 26; c6++) masks.emplace_back((1 << c1) | (1 << c2) | (1 << c3) | (1 << c4) | (1 << c5) | (1 << c6));

    int t;
    cin >> t;

    while (t--) {
        int l;
        cin >> l;

        int m0 = 0;
        vector<int> adj_mask_regular(26, 0), adj_mask_transpose(26, 0);
        while (l--) {
            string s;
            cin >> s;

            int c1 = s[0] - 'a';
            m0 |= 1 << c1;
            if (s.size() > 1) {
                int c2 = s[1] - 'a';
                adj_mask_regular[c1] |= 1 << c2;
                adj_mask_transpose[c2] |= 1 << c1;
            }
        }

        vector<vector<int>> suff(26, vector<int>(27, 0));
        for (int c1 = 0; c1 < 26; c1++)
            for (int c2 = 25; ~c2; c2--) suff[c1][c2] = suff[c1][c2 + 1] + ((adj_mask_regular[c1] >> c2) & 1);

        vector<int> degree(26, 0), add(6, 0);
        int score = -1, m1 = 0, m2 = 0;
        auto dfs = [&](auto &&self, int ch = 0, int chosen = 0) -> void {
            if (chosen == 6) {
                int s = 0;
                for (int m3 = m2 & m0; m3; m3 &= m3 - 1) s += max(3, degree[countr_zero((unsigned) m3)]);

                if (score < s) {
                    score = s;
                    m1 = m2;
                }
                return;
            }
            if (20 < ch - chosen) return;

            int remaining = 6 - chosen, bound = 0;
            for (int m3 = m2 & m0; m3; m3 &= m3 - 1) {
                int c = countr_zero((unsigned) m3);
                bound += max(3, degree[c] + min(remaining, suff[c][ch]));
            }

            if (remaining) {
                fill(add.begin(), add.end(), -1);
                for (int c = ch; c < 26; c++)
                    if ((m0 >> c) & 1) {
                        int value = max(3, degree[c] + min(remaining - 1, suff[c][ch]));
                        for (int i = 0; i < remaining; i++)
                            if (add[i] < value) {
                                for (int j = remaining - 1; j > i; j--) add[j] = add[j - 1];
                                add[i] = value;
                                break;
                            }
                    }

                for (int i = 0; i < remaining; i++)
                    if (~add[i]) bound += add[i];
            }
            if (score >= bound) return;

            for (int c = ch; c <= chosen + 20; c++) {
                if (score >= bound) return;
                m2 |= 1 << c;
                for (int m3 = adj_mask_transpose[c]; m3; m3 &= m3 - 1) degree[countr_zero((unsigned) m3)]++;
                self(self, c + 1, chosen + 1);
                for (int m3 = adj_mask_transpose[c]; m3; m3 &= m3 - 1) degree[countr_zero((unsigned) m3)]--;
                m2 ^= 1 << c;
            }
        };
        dfs(dfs);

        string s;
        for (int c = 0; c < 26; c++)
            if ((m1 >> c) & 1) s += (char) ('a' + c);
        cout << fixed << setprecision(6) << s << "\n" << score / 18. << "\n";
    }
}
