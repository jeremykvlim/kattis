#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int carry = 0;
    reverse(s.begin(), s.end());
    for (char &c : s) {
        int d = c - '0' + carry;
        c = '0' + (d & 1);
        carry = d / 2;
    }
    for (; carry; carry /= 2) s += '0' + (carry & 1);
    reverse(s.begin(), s.end());
    cout << s;
}