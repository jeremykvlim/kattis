#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    auto sum = 0LL, count = 0LL, total = 1LL;
    for (char c : s) {
        if (c == '1') count = (count + total) % MODULO;
        else if (c == '?') {
            sum = ((sum * 2) % MODULO + count) % MODULO;
            count = ((count * 2) % MODULO + total) % MODULO;
            total = (total * 2) % MODULO;
        }
        else sum = (sum + count) % MODULO;
    }
    
    cout << sum;
}
