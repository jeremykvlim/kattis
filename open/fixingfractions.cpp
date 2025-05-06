#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, c, d;
    cin >> a >> b >> c >> d;

    auto sa = to_string(a), sb = to_string(b);
    auto g = __gcd(c, d);
    c /= g;
    d /= g;

    for (int mask = 0; mask < 1 << sa.size(); mask++) {
        auto A = 0LL;
        vector<int> count(10, 0);
        for (int i = 0; i < sa.size(); i++)
            if ((mask >> i) & 1) {
                if (!A && sa[i] == '0') break;
                A *= 10;
                A += sa[i] - '0';
            } else count[sa[i] - '0']++;

        if (!A || A % c || b < (__int128) A / c * d) continue;

        auto B = A / c * d;
        auto sB = to_string(B);

        int j = 0;
        for (int i = 0; i < sb.size(); i++)
            if (j != sB.size() && sb[i] == sB[j]) j++;
            else count[sb[i] - '0']--;

        if (j != sB.size() || !all_of(count.begin(), count.end(), [&](int c) { return !c; })) continue;
        cout << "possible\n" << A << " " << B;
        exit(0);
    }
    cout << "impossible";
}
