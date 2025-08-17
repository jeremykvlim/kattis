#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    auto count = 0LL;
    for (auto i = 1LL; i <= n;) {
        auto quo = n / i, rem = n / quo;
        count += quo * (rem - i + 1);
        i = rem + 1;
    }
    cout << count;
}
