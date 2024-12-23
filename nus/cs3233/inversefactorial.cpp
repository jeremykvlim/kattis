#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int len = s.size();
    if (len <= 3) {
        int f = stoi(s);
        if (f <= 2) cout << f;
        else if (f == 6) cout << 3;
        else if (f == 24) cout << 4;
        else if (f == 120) cout << 5;
        else cout << 6;
    } else {
        int n = 1;
        for (double i = 0; i < len - 1; i += log10(++n));
        cout << n;
    }
}
