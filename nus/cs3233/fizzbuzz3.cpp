#include <bits/stdc++.h>
using namespace std;

const string F0 = "F_in_Finals_stands_for_Fizz_Buzz!";

char c(vector<long long> &p3, long long i, long long pos) {
    if (!i) return 'F';
    auto len = 16 * p3[i - 1] - 15;

    if (pos < len) return c(p3, i - 1, pos);
    pos -= len;

    if (pos < 4) return F0[1 + pos];
    pos -= 4;

    if (pos < len) return c(p3, i - 1, pos);
    pos -= len;

    if (pos < 17) return F0[6 + pos];
    pos -= 17;

    if (pos < len) return c(p3, i - 1, pos);
    pos -= len;

    return pos < 9 ? F0[24 + pos] : '?';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> p3(36, 1);
    for (int i = 1; i < 36; i++) p3[i] = 3 * p3[i - 1];

    int q;
    cin >> q;

    while (q--) {
        long long x, y;
        cin >> x >> y;

        cout << c(p3, min(36LL, y + 1), x - 1);
    }
}
