#include <bits/stdc++.h>
using namespace std;

bool isbinarypalindrome(long long n) {
    auto rev = 0LL;
    for (auto temp = n; temp; temp >>= 1) rev = (rev << 1) | (temp & 1);
    return n == rev;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    int count = 1;
    for (auto i = 1LL;; i++) {
        auto s = to_string(i), rev = s;
        reverse(rev.begin(), rev.end());

        auto odd = stoll(s + rev.substr(1)), even = stoll(s + rev);
        if (odd >= n) break;
        count += isbinarypalindrome(odd);
        if (even >= n) continue;
        count += isbinarypalindrome(even);
    }
    cout << count;
}
