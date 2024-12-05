#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, a, b;
    while (cin >> t >> a >> b) {
        cout << "(" << t << "^" << a << "-1)/(" << t << "^" << b << "-1) ";

        if (t == 1 || a < b || a % b) cout << "is not an integer with less than 100 digits.\n";
        else {
            vector<int> digits(100, 0);
            for (int i = a - b; i >= 0; i -= b) {
                vector<long long> temp(100, 0);
                temp[99] = 1;

                for (int k = 0; k < i; k++) {
                    for (int j = 99; j; j--) temp[j] *= t;
                    for (int j = 99; j; j--)
                        if (temp[j] > 9) {
                            temp[j - 1] += temp[j] / 10;
                            temp[j] %= 10;
                        }

                    if (temp[0]) {
                        cout << "is not an integer with less than 100 digits.\n";
                        goto next;
                    }
                }

                for (int j = 99; j; j--) {
                    digits[j] += temp[j];
                    if (digits[j] > 9) {
                        digits[j - 1] += digits[j] / 10;
                        digits[j] %= 10;
                    }
                }
            }

            bool lz = true;
            for (int i = 1; i <= 99; i++) {
                if (digits[i]) lz = false;
                if (!lz) cout << digits[i];
            }
            cout << "\n";
        }
        next:;
    }
}
