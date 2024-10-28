#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> fact(10, 1);
    for (int i = 2; i < 10; i++) fact[i] = i * fact[i - 1];

    int n;
    cin >> n;

    string s;
    for (int i = fact.size() - 1, x; ~i; i--) {
        x = n / fact[i];
        n %= fact[i];
        if (x) s += string(x, '0' + i - 1 + (i > 1));
    }
    reverse(s.begin(), s.end());
    if (s.size() > 1 && s[0] == '0') s[0]++;

    cout << s;
}
