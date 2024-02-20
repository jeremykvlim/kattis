#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    string s;
    cin >> k >> s;

    vector<int> pref(s.size() + 1, 0);
    for (int i = 0; i < s.size(); i++) pref[i + 1] = pref[i] + (s[i] == '1');

    int l = k, f = 0, diff = 0;
    for (int i = k; i < 2 * k; i++)
        for (int j = 0; i + j <= s.size(); j++)
            if (diff * i < l * (pref[i + j] - pref[j])) {
                diff = pref[i + j] - pref[j];
                l = i;
                f = j;
            }

    cout << f + 1 << " " << l;
}
