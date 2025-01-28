#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(const string &s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

vector<vector<int>> automaton(string s, const vector<int> &pi) {
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
    auto pi = prefix_function(p2);
    auto fsm = automaton(p2, pi);
    vector<vector<vector<vector<vector<long long>>>>> memo(n.size(), vector<vector<vector<vector<long long>>>>(n.size(), vector<vector<vector<long long>>>(2, vector<vector<long long>>(2, vector<long long>(2, -1)))));
    auto dp = [&](auto &&self, int i = 0, int state = 0, bool match = false, bool bound = true, bool zero = true) -> long long {
        if (i == n.size()) return zero ? 0 : match;
        if (~memo[i][state][match][bound][zero]) return memo[i][state][match][bound][zero];

        auto count = 0LL;
        int limit = bound ? n[i] - '0' : 9;
        for (int d = 0; d <= limit; d++) {
            int s = state;
            bool m = match, b = bound && d == (n[i] - '0'), z = zero && !d;
            if (!z)
                if (!match) {
                    s = fsm[state][d];
                    if (s == p2.size()) m = true;
                }
            count += self(self, i + 1, s, m, b, z);
        }
        return memo[i][state][match][bound][zero] = count;
    };
    cout << dp(dp);
}
