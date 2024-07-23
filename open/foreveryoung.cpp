#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long y, l;
    cin >> y >> l;

    for (int i = log10(l);; i++)
        for (int d = 1; d <= 9; d++)
            for (long long br = powl(y / d, 1.L / i), bl = max(powl((y - 9 * (powl(br, i) - 1) / (br - 1)) / d, 1.L / i), 10.L); br >= bl; br--) {
                auto age = 0LL, p10 = 1LL, temp = y;
                while (temp) {
                    auto r = temp % br;
                    if (r > 9) goto next;
                    age += r * p10;
                    p10 *= 10;
                    temp /= br;
                }

                if (age >= l) {
                    cout << br;
                    exit(0);
                }

                next:;
            }
}
