#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto len = 0LL;
    while (n--) {
        char ch;
        cin >> ch;

        int c = ch - 'a';
        len = min((((len >> c) + 1) | 1) << c, (((len >> (c + 26)) + 1) | 1) << (c + 26));
    }
    cout << len % 998244353;
}
