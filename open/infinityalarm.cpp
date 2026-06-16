#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << 1;
        exit(0);
    }

    vector<int> s(n);
    for (int &si : s) {
        cin >> si;
        si--;
    }
    sort(s.begin(), s.end());

    int seconds = 0;
    for (int b = 29; ~b; b--) {
        int mask = 1 << b, m = lower_bound(s.begin(), s.end(), mask) - s.begin();
        if (m == s.size()) continue;

        vector<int> temp;
        for (int l = 0, r = m; l < m || r < s.size();)
            if (r == s.size()) temp.emplace_back(s[l++]);
            else if (l == m) temp.emplace_back(s[r++] ^ mask);
            else {
                int left = s[l], right = s[r] ^ mask;
                if (left < right) {
                    temp.emplace_back(left);
                    l++;
                } else if (left > right) {
                    temp.emplace_back(right);
                    r++;
                } else {
                    l++;
                    r++;
                }
            }

        if (temp.empty()) {
            seconds |= mask;
            for (int i = m; i < s.size(); i++) temp.emplace_back(s[i] ^ mask);
        }
        s = temp;
    }
    cout << seconds + 1;
}
