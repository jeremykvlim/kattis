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
        string line;
        cin >> line;
        
        double radix = 0, number = 0;
        int state = 1;
        bool valid = true;
        
        for (char c : line) {
            if (!valid) break;
            int digit = ascii[c];
            switch (state) {
                case 1:
                    valid = digit >= 0 && digit <= 9;
                    if (valid) number = digit, state = 2;
                    break;
                case 2:
                    if (c == '#') valid = number >= 2 && number <= 16, radix = number, number = 0, state = 3;
                    else valid = digit >= 0 && digit <= 9, number = number * 10 + digit;
                    break;
                case 3:
                    valid = digit >= 0 && digit < radix;
                    if (valid) number = digit, state = 4;
                    break;
                case 4:
                    if (c == '#') state = 5;
                    else valid = digit >= 0 && digit < radix, number = number * radix + digit;
                    break;
                case 5:
                    valid = c == '#' && number >= 2 && number <= 16, radix = number, number = 0, state = 3;
                    break;
            }
        }
        cout << (valid && (state == 2 || state == 5) ? "yes" : "no") << "\n";
    }
}
