#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int pref_len = 25;
    for (int i = 1; i <= 999; i++) pref_len += to_string(i).size();

    int r = 1;
    for (int len = 0; len < pref_len; r++) len += to_string(r).size();

    string champernowne;
    for (int i = 1; i < r; i++) champernowne += to_string(i);

    vector<__int128> p10(27, 1), sum(27, 0);
    for (int d = 1; d <= 26; d++) {
        p10[d] = p10[d - 1] * 10;
        sum[d] = sum[d - 1] + p10[d - 1] * 9 * d;
    }

    int t;
    cin >> t;

    while (t--) {
        string s;
        cin >> s;
        int n = s.size();

        auto index = ((__int128) 1) << 100;
        for (int i = 0; i + n < champernowne.size(); i++) {
            if (mismatch(s.begin(), s.end(), champernowne.begin() + i, [](char c1, char c2) { return c1 == '?' || c1 == c2; }).first != s.end()) continue;
            index = i + 1;
            break;
        }

        if (index < 1e8) {
            cout << (int) index << "\n";
            continue;
        }

        auto stoui128 = [&](const string &s) {
            __int128 x = 0;
            for (char d : s) x = x * 10 + (d - '0');
            return x;
        };

        for (int len = 4; len <= n + 1; len++)
            for (int offset = 1; offset <= len; offset++)
                for (int d = 0; d <= 9; d++)
                    for (int consec_9s = -1; consec_9s < len; consec_9s++)
                        if ((offset + len * (9 - d) < n) == !!~consec_9s) {
                            string S(len - 1, '?');
                            S += (char) (d + '0');

                            if (consec_9s > 0)
                                for (int i = consec_9s - 1; ~i; i--) S[len - i - 2] = '9';

                            for (int o = offset, count = 0; char c : s) {
                                if (!o) o = len + (++count + d >= 10 && consec_9s + 1 == len);

                                if (c != '?') {
                                    if (o == 1) c = (char) ((c - '0' + 10 - count) % 10 + '0');
                                    else if (count + d >= 10 && consec_9s + 2 >= o) c = (char) ((c - '0' + 9) % 10 + '0');

                                    if (o > len) {
                                        if (c != '0') goto next;
                                    } else if (S[len - o] == '?') S[len - o] = c;
                                    else if (S[len - o] != c) goto next;
                                }
                                o--;
                            }
                            if ((~consec_9s && consec_9s != len - 1 && S[len - consec_9s - 2] == '9') || S[0] == '0') goto next;

                            if (S[0] == '?') S[0] = '1';
                            for (int i = 1; i < len; i++)
                                if (S[i] == '?') S[i] = '0';

                            index = min(index, sum[len - 1] + (stoui128(S) - p10[len - 1]) * len + (len - offset + 1));
                            next:;
                        }

        cout << (int) (index % 998244353) << "\n";
    }
}
