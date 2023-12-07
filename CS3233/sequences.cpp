#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int mod(long long x) {
    return x % MODULO;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    cin >> s;
    long long sum = 0, count = 0, total = 1;
    for (char c : s) {
        if (c == '1') count = mod(count + total);
        else if (c == '?') {
            sum = mod(mod(sum * 2) + count);
            count = mod(mod(count * 2) + total);
            total = mod(total * 2);
        }
        else sum = mod(sum + count);
    }
    cout << sum;
}
