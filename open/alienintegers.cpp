#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    vector<int> count(10, 0), digits;
    for (auto temp = n; temp; temp /= 10) {
        digits.emplace_back(temp % 10);
        count[digits.back()]++;
    }

    if (none_of(count.begin(), count.end(), [&](int c) {return !c;})) {
        cout << "Impossible";
        exit(0);
    }

    int dl = 10, dr = -1, dm = 10;
    for (int d = 0; d <= 9; d++)
        if (!count[d]) {
            dl = min(dl, d);
            dr = max(dr, d);
            if (d < dm && d) dm = d;
        }

    auto l = 0LL, r = 0LL;
    for (int i = 0; i < digits.size(); i++)
        if (i == digits.size() - 1) {
            for (int d = digits[i]; d <= 9; d++) {
                if (!count[d]) {
                    r += d * pow(10, i);
                    break;
                }

                if (d == 9) r += dl * pow(10, i) + dm * pow(10, i + 1);
            }
            for (int d = digits[i]; ~d; d--)
                if (!count[d]) {
                    l += d * pow(10, i);
                    break;
                }
        } else {
            l += dr * pow(10, i);
            r += dl * pow(10, i);
        }

    if (abs(n - l) == abs(n - r)) cout << l << " " << r;
    else if (abs(n - l) < abs(n - r)) cout << l;
    else cout << r;
}
