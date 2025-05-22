#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> kmp_automaton(string s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    s += '{';
    vector<vector<int>> fsm(s.size(), vector<int>(10, 0));
    for (int i = 0; i < s.size(); i++)
        for (int d = 0; d < 10; d++)
            fsm[i][d] = (i && '0' + d != s[i]) ? fsm[pi[i - 1]][d] : i + ('0' + d == s[i]);
    return fsm;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string n, x, y, z;
    cin >> n >> x >> y >> z;
    reverse(n.begin(), n.end());
    reverse(x.begin(), x.end());
    reverse(y.begin(), y.end());
    reverse(z.begin(), z.end());

    vector<string> s{x, y, z};
    vector<vector<vector<int>>> automata(3);
    for (int i = 0; i < 3; i++) {
        automata[i] = kmp_automaton(s[i]);
        for (int j = 0; j < 10; j++) automata[i].back()[j] = automata[i].size() - 1;
    }
    sort(automata.begin(), automata.end());

    int m = n.size();
    vector<int> digits(m);
    vector<long long> p10(m + 1, 1);
    for (int p = 0; p < m; p++) {
        digits[p] = n[p] - '0';
        p10[p + 1] = p10[p] * 10;
    }

    vector<int> digit_c(3 * 10 * 10 * 10), quo(3 * 10 * 10 * 10);
    auto index1 = [&](int i, int da, int db, int r) {
        return ((i * 10 + da) * 10 + db) * 10 + r;
    };
    for (int i = 0; i < 3; i++)
        for (int da = 0; da < 10; da++)
            for (int db = 0; db < 10; db++)
                for (int dc = 0; dc < 10; dc++)
                    for (int r = 0; r < 10; r++)
                        if ((i + da + db + dc) % 10 == r) {
                            int j = index1(i, da, db, r);
                            digit_c[j] = dc;
                            quo[j] = (i + da + db + dc) / 10;
                        }

    auto diff = LLONG_MAX;
    do {
        int s1 = automata[0].size(), s2 = automata[1].size(), s3 = automata[2].size();
        vector<long long> dp((m + 1) * s1 * s2 * s3 * 2 * 2 * 3, LLONG_MAX);
        dp[0] = 0;
        auto index2 = [&](int p, int ka, int kb, int kc, int ab = 0, int ac = 0, int i = 0) {
            return (((((p * s1 + ka) * s2 + kb) * s3 + kc) * 2 + ab) * 2 + ac) * 3 + i;
        };
        for (int p = 0; p < m; p++)
            for (int ka = 0; ka < s1; ka++)
                for (int kb = 0; kb < s2; kb++)
                    for (int kc = 0; kc < s3; kc++)
                        for (int ab = 0; ab < 2; ab++)
                            for (int bc = 0; bc < 2; bc++)
                                for (int i = 0; i < 3; i++) {
                                    auto d = dp[index2(p, ka, kb, kc, ab, bc, i)];
                                    if (d < diff)
                                        for (int da = 0; da < 10; da++)
                                            for (int db = 0; db < 10; db++) {
                                                int j = index1(i, da, db, digits[p]), dc = digit_c[j],
                                                    k = index2(p + 1, automata[0][ka][da], automata[1][kb][db], automata[2][kc][dc],
                                                               da > db || (da == db && ab), db > dc || (db == dc && bc), quo[j]);
                                                dp[k] = min(dp[k], d + (dc - da) * p10[p]);
                                            }
                                }
        diff = min(diff, dp[index2(m, s1 - 1, s2 - 1, s3 - 1)]);
    } while (next_permutation(automata.begin(), automata.end()));
    cout << (diff == LLONG_MAX ? -1 : diff);
}
