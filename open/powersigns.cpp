#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    s = '0' + s;
    for (int i = s.size() - 1; ~i; i--) {
        while (s[i] > '1') {
            s[i - 1]++;
            s[i] -= 2;
        }

        if (1 < i && i < s.size() - 1 && s[i] == '1' && s[i + 1] == '1') {
            s[i - 1]++;
            s[i] = '0';
            s[i + 1] = '-';
        }
    }
    if (s[0] == '0') s.erase(s.begin());
    for (char &c : s)
        if (c == '1') c = '+';
    cout << s;
}
