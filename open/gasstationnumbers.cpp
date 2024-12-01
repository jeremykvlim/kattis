#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    while (cin >> s && s != ".") {
        s.erase(s.length() - 2, 1);
        int n = s.length();

        int i = n - 1;
        for (; ~i; i--)
            for (char c = s[i] + 1; c <= '9'; c++)
                for (int j = i; j < n; j++)
                    if (s[j] == c || (s[j] == '2' && c == '5') || (s[j] == '5' && c == '2') || (s[j] == '6' && c == '9') || (s[j] == '9' && c == '6')) {
                        s[j] = s[i];
                        s[i] = c;
                        goto done;
                    }
        cout << "The price cannot be raised\n";
        continue;
        done:;
        for (int j = i + 1; j < n; j++) {
            if (s[j] == '5') s[j] = '2';
            else if (s[j] == '9') s[j] = '6';
        }
        sort(s.begin() + i + 1, s.end());
        for (int j = 0; j < n - 1; j++) cout << s[j];
        cout << "." << s[n - 1] << "\n";
    }
}
