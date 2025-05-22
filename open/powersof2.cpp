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

    string n;
    int e;
    cin >> n >> e;

    auto p2 = to_string(1LL << e);
    auto fsm = kmp_automaton(p2);

    int s = n.size();
    vector<long long> memo(s * s * 2 * 2 * 2, -1LL);
    auto index = [&](int p, int state, bool match, bool bound, bool zero) {
        return ((((p * s + state) << 1 | match) << 1 | bound) << 1 | zero);
    };
    auto dp = [&](auto &&self, int p = 0, int state = 0, bool match = false, bool bound = true, bool zero = true) -> long long {
        if (p == n.size()) return zero ? 0 : match;

        int i = index(p, state, match, bound, zero);
        if (~memo[i]) return memo[i];

        auto count = 0LL;
        int d_max = bound ? n[p] - '0' : 9;
        for (int d = 0; d <= d_max; d++) {
            int s = state;
            bool m = match, b = bound && d == (n[p] - '0'), z = zero && !d;
            if (!z)
                if (!match) {
                    s = fsm[state][d];
                    if (s == p2.size()) m = true;
                }
            count += self(self, p + 1, s, m, b, z);
        }
        return memo[i] = count;
    };
    cout << dp(dp);
}
