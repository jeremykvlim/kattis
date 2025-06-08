#include <bits/stdc++.h>
using namespace std;

vector<int> manacher(const string &s) {
    int n = s.size();
    if (n <= 1) return {};

    vector<int> dp(n, 0);
    for (int k = 0, l = -1, r = -1; k < 2 * n - 1; k += 2) {
        int i = (k + 1) >> 1, j = k >> 1, p = i >= r ? 0 : min(r - i, dp[l + r - i]);
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) p++;

        if (r < j + p) {
            r = j + p;
            l = i - p;
        }

        dp[i] = p;
    }
    return dp;
}

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    string store, board;
    cin >> n >> t >> store >> board;

    auto store_rev = store;
    auto board_rev = board;
    reverse(store_rev.begin(), store_rev.end());
    reverse(board_rev.begin(), board_rev.end());

    auto dp_pref = manacher(store), dp_suff = manacher(store_rev);
    vector<bool> pref(n, false), suff(n, false);
    pref[0] = suff[0] = true;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (dp_pref[i] >= i - l) {
            pref[i] = true;
            l = i;
        }
        if (dp_suff[i] >= i - r) {
            suff[i] = true;
            r = i;
        }
    }

    auto possible = [&](const string &s) -> bool {
        auto pi = prefix_function(s);

        int m = s.size();
        for (int i = 0, j = 0; i < n; i++) {
            while (j && store[i] != s[j]) j = pi[j - 1];
            if (store[i] == s[j]) j++;
            if (j == m) {
                int l = i - m + 1, r = n - i - 1;
                if (pref[l] && suff[r]) return true;
                j = pi[j - 1];
            }
        }
        return false;
    };

    cout << (possible(board) || possible(board_rev) ? "possible" : "impossible");
}
