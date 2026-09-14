#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    bool f = true;
    for (char c : s)
        if (islower(c)) f = false;

    bool m = isupper(s[0]) && s.back() == '.';
    for (int i = 1; i < s.size() - 1; i++)
        if (isupper(s[i])) m = false;

    if (f && m) cout << "Óljóst";
    else if (f) cout << "Doktor Frankenstein";
    else if (m) cout << "Skrímsli Frankensteins";
    else cout << "Einhver annar";
}