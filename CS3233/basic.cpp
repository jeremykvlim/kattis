#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> ascii(104, -1);
    for (int i = 0; i <= 9; i++) ascii[i + '0'] = i;
    for (int i = 0; i <= 5; i++) ascii[i + 'a'] = i + 10;

    int n;
    cin >> n;

    while (n--) {
        string s;
        cin >> s;

        double radix = 0, num = 0;
        int state = 1;
        bool valid = true;
        for (char c : s) {
            if (!valid) break;

            int digit = ascii[c];
            switch (state) {
                case 1:
                    valid = 0 <= digit && digit <= 9;
                    if (valid) {
                        num = digit;
                        state = 2;
                    }
                    break;
                case 2:
                    if (c == '#') {
                        valid = 2 <= num && num <= 16;
                        radix = num;
                        num = 0;
                        state = 3;
                    }
                    else {
                        valid = 0 <= digit && digit <= 9;
                        num = num * 10 + digit;
                    }
                    break;
                case 3:
                    valid = 0 <= digit && digit < radix;
                    if (valid) {
                        num = digit;
                        state = 4;
                    }
                    break;
                case 4:
                    if (c == '#') state = 5;
                    else {
                        valid = 0 <= digit && digit < radix;
                        num = num * radix + digit;
                    }
                    break;
                case 5:
                    valid = c == '#' && 2 <= num && num <= 16;
                    radix = num;
                    num = 0;
                    state = 3;
                    break;
            }
        }

        cout << (valid && (state == 2 || state == 5) ? "yes\n" : "no\n");
    }
}
