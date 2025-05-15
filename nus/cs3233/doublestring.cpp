#include <bits/stdc++.h>
using namespace std;

vector<int> z_function(string s) {
    vector<int> z(s.size());
    for (int i = 1, l = 0, r = 0; i < s.size(); i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto count = 0LL;
    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 == r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);
        self(self, m, r);

        for (int k : {0, 1}) {
            auto sl = s.substr(l, m - l), sr = s.substr(m, r - m);

            auto z1 = z_function(sr + sl);
            z1 = {z1.end() - (m - l), z1.end()};

            reverse(sl.begin(), sl.end());
            auto z2 = z_function(sl);
            reverse(z2.begin(), z2.end());

            for (int len = 1; len <= m - l; len++) {
                int p = m - len - l;
                count += max(0, min(len - k, z1[p]) - max(1, len - (p ? z2[p - 1] : 0)) + 1);
            }
            m = l + r - m;
            reverse(s.begin() + l, s.begin() + r);
        }
    };
    dnc(dnc, 0, s.size());
    cout << count;
}
