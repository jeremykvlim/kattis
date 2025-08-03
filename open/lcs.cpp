#include <bits/stdc++.h>
using namespace std;

string longest_common_subsequence(const string &a, const string &b) {
    string lcs;

    auto masks = [&](const string &s, int l, int r) {
        int m = r - l, w = (m + 63) >> 6;
        vector<vector<unsigned long long>> masks(26, vector<unsigned long long>(w, 0));
        for (int i = 0; i < m; i++) masks[s[i + l] - 'a'][i >> 6] |= 1ULL << (i & 63);
        return masks;
    };

    auto dp = [&](const string &s, int l, int r, const auto &masks) {
        int w = masks[0].size();
        vector<unsigned long long> dp(w, 0), x(w), y(w), z(w);
        for (int k = l; k < r; k++) {
            int pos = s[k] - 'a';
            for (int i = 0; i < w; i++) x[i] = dp[i] | masks[pos][i];

            bool carry = true;
            for (int i = 0; i < w; i++) {
                y[i] = (dp[i] << 1) | carry;
                carry = dp[i] >> 63;
            }

            bool borrow = false;
            for (int i = 0; i < w; i++) {
                z[i] = x[i] - y[i] - borrow;
                borrow = (x[i] < y[i] + borrow);
            }

            for (int i = 0; i < w; i++) dp[i] = x[i] & ~z[i];
        }
        return dp;
    };

    auto pref_dp = [&](const auto &dp, int m) {
        vector<int> pref(m + 1, 0);
        for (int i = 0, j = 0; i < dp.size(); i++)
            for (int b = 0; b < 64 && j < m; b++, j++) pref[j + 1] = pref[j] + ((dp[i] >> b) & 1);
        return pref;
    };

    auto hirschberg = [&](auto &&self, const string &a, int al, int ar, const string &b, int bl, int br) {
        if (al >= ar || bl >= br) return;

        if (al + 1 == ar) {
            if (any_of(b.begin() + bl, b.begin() + br, [&](char c) { return c == a[al]; })) lcs += a[al];
            return;
        }
        int am = al + (ar - al) / 2, len_a = ar - am, len_b = br - bl;
        auto a_rev = a.substr(am, len_a);
        reverse(a_rev.begin(), a_rev.end());
        auto b_rev = b.substr(bl, len_b);
        reverse(b_rev.begin(), b_rev.end());

        auto pref = pref_dp(dp(a, al, am, masks(b, bl, br)), len_b), pref_rev = pref_dp(dp(a_rev, 0, len_a, masks(b_rev, 0, len_b)), len_b);
        int k = 0;
        for (int i = 0, longest = -1; i <= len_b; i++) {
            int len = pref[i] + pref_rev[len_b - i];
            if (longest < len) {
                longest = len;
                k = i;
            }
        }

        self(self, a, al, am, b, bl, bl + k);
        self(self, a, am, ar, b, bl + k, br);
    };
    hirschberg(hirschberg, a, 0, a.size(), b, 0, b.size());
    return lcs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    cin >> a >> b;

    auto lcs = longest_common_subsequence(a, b);
    cout << lcs.size() << "\n" << lcs;
}
