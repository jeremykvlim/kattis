#include <bits/stdc++.h>
using namespace std;

bool ispalindrome(const string &s) {
    return s == string(s.rbegin(), s.rend());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    vector<vector<int>> prev(n, vector<int>(26, -1));
    for (int i = 0; i < n; i++) {
        if (i) prev[i] = prev[i - 1];
        prev[i][s[i] - 'a'] = i;
    }

    auto check = [&](int m) -> pair<int, int> {
        for (int L = 0; L + m <= n; L++) {
            int R = L + m - 1;
            auto t = s.substr(L, m);

            int l = 0, r = m - 1;
            for (; l < r && t[l] == t[r]; l++, r--);
            if (l >= r) return {-1, -1};

            auto insertions = [&](const auto &t) {
                int l = 0, r = t.size() - 1;
                for (; l < r && t[l] == t[r]; l++, r--);

                vector<pair<char, int>> v;
                if (l >= r) {
                    v.emplace_back(t[l], l);
                    v.emplace_back(t[l], l + 1);
                } else {
                    if (ispalindrome(t.substr(l, r - l))) v.emplace_back(t[r], l);
                    if (ispalindrome(t.substr(l + 1, r - l))) v.emplace_back(t[l], r + 1);
                }
                return v;
            };

            for (int i : {l, r}) {
                auto temp = t;
                char ch = temp[i];
                temp.erase(temp.begin() + i);
                for (auto [c, j] : insertions(temp))
                    if (c == ch && i < j) return {L + i, L + j};
            }

            if (L) {
                t = s.substr(L + 1, m - 1);
                for (auto [c, j] : insertions(t)) {
                    int i = prev[L - 1][c - 'a'];
                    if (~i) return {i, L + j};
                }
            }

            if (R + 1 < n) {
                t = s.substr(L, m + 1);
                l = 0;
                r = t.size() - 1;
                for (; l < r && t[l] == t[r]; l++, r--);

                if (l >= r) {
                    if (l < m) return {L + l, R + 1};
                } else {
                    if (ispalindrome(t.substr(l + 1, r - l)) && l < m) return {L + l, R + 1};
                    if (ispalindrome(t.substr(l, r - l)) && r < m) return {L + r, R + 1};
                }
            }
        }
        return {-2, -2};
    };

    int l = 0, r = n / 2 + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (check(2 * m) != make_pair(-2, -2)) l = m;
        else r = m;
    }

    if (!l) {
        cout << -1;
        exit(0);
    }

    cout << 2 * l << "\n";
    auto [L, R] = check(2 * l);
    if (L == -1) cout << -1;
    else cout << L + 1 << " " << R + 1;
}
