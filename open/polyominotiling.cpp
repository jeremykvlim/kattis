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

pair<int, int> gambini_vuillon(string &b) {
    int n = b.size();
    if (n & 1) return {0, 0};

    b += b;
    auto bar = [&](const char &c) {
        if (c == 'u') return 'd';
        if (c == 'd') return 'u';
        if (c == 'l') return 'r';
        return 'l';
    };
    int PS = 0, PH = 0;
    for (int i = 1; i < n; i++) {
        if (b[0] == bar(b[i])) {
            int x1 = 0, y1 = 1, x2 = i, y2 = i + 1;
            while (b[x1 - 1 + n] == bar(b[y2 + n])) {
                x1--;
                y2++;
            }
            while (b[y1 + n] == bar(b[x2 - 1 + n])) {
                y1++;
                x2--;
            }
            int m = x2 - y1;
            if (m - n != x1 - y2 || m <= 0) continue;

            auto U = b.substr(y1 + n, m), V = b.substr(y2 + n, m);
            PS += equal(U.begin(), U.end(), V.rbegin(), [&](const char &c1, const char &c2) { return c1 == bar(c2); });
            if (m > 1) {
                string U_bar(m, '#');
                transform(U.begin(), U.end(), U_bar.rbegin(), [&](char c) { return bar(c); });

                auto VV = V + V;
                auto pi = prefix_function(U_bar);
                for (int k = 0, j = 0; k < 2 * m; k++) {
                    while (j && VV[k] != U_bar[j]) j = pi[j - 1];
                    if (VV[k] == U_bar[j]) j++;
                    if (j == m) {
                        PH += m <= k && k < 2 * m - 1;
                        j = pi[j - 1];
                    }
                }
            }
        }
    }
    return {PS, PH};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    string s;
    cin >> k >> s;

    auto [PS, PH] = gambini_vuillon(s);
    cout << 4 * PS + 6 * PH;
}
