#include <bits/stdc++.h>
using namespace std;

int manacher(string s) {
    int n = s.size();
    if (n <= 1) return n;

    vector<int> dp(2 * n - 1, 0);
    int len = 0;
    for (int k = 0, l = -1, r = -1; k < 2 * n - 1; k++) {
        int i = (k + 1) >> 1, j = k >> 1, p = i >= r ? 0 : min(r - i, dp[2 * (l + r) - k]);
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) p++;

        if (r < j + p) {
            r = j + p;
            l = i - p;
        }

        dp[k] = p;
        len = max(len, 2 * dp[k] + !(k & 1));
    }

    return len;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, p;
    cin >> n >> k >> p;

    if (n < k || (n > 1 && n == k && n == p)) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    switch (k) {
        case 1: {
            cout << (n != p ? "IMPOSSIBLE" : string(n, 'a'));
            break;
        }

        case 2: {
            if (n == p) {
                cout << 'a' << string(n - 2, 'b') << 'a';
                break;
            }

            if (n <= 8) {
                auto search = [&](auto &&self, string s = "", int i = 0) -> void {
                    if (i == n) {
                        if (manacher(s) == p) {
                            cout << s;
                            exit(0);
                        };

                        return;
                    }

                    if (s != string(n - 1, 'a')) self(self, s + 'a', i + 1);
                    if (s != string(n - 1, 'b')) self(self, s + 'b', i + 1);
                };
                search(search);
                cout << "IMPOSSIBLE";
                break;
            }

            if (p <= 3) cout << "IMPOSSIBLE";
            else {
                string s(p, 'b');
                for (int i = 0; s.size() < n; i = (i + 1) & 1)
                    for (char c : {'a', 'b'})
                        for (int j = i; ~j && s.size() < n; j--) s += c;

                cout << s;
            }
            break;
        }

        default: {
            vector<bool> used(26, false);
            string s;
            for (char c = 'a'; s.size() < (p + (p & 1)) / 2; c = 'a' + (c - 'a' + 1) % k) {
                s += c;
                used[c - 'a'] = true;
            }
            for (int i = s.size() - 1 - (p & 1); ~i; i--) s += s[i];
            for (char c = 'a' + k - 1; s.size() < n; c = 'a' + (c - 'a' - 1 + k) % k) {
                s += c;
                used[c - 'a'] = true;
            }
            cout << (count(used.begin(), used.end(), true) == k ? s : "IMPOSSIBLE");
            break;
        }
    }
}
