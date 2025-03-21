#include <bits/stdc++.h>
using namespace std;

bool ispalindrome(const string &s) {
    return s == string(s.rbegin(), s.rend());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string a, b;
    while (getline(cin, a) && getline(cin, b)) {
        if (a == b) {
            cout << "No solution.\n";
            next:;
            continue;
        }

        if (a.size() > b.size()) swap(a, b);

        for (int i = 0; i <= a.size(); i++) {
            auto xa = a.substr(0, i), xb = b.substr(0, i);
            reverse(xa.begin(), xa.end());
            reverse(xb.begin(), xb.end());

            if (xa > xb) swap(xa, xb);

            if (ispalindrome(a + xa) ^ ispalindrome(b + xa)) {
                cout << xa + "\n";
                goto next;
            }

            if (ispalindrome(a + xb) ^ ispalindrome(b + xb)) {
                cout << xb + "\n";
                goto next;
            }
        }

        reverse(a.begin(), a.end());
        cout << (b[a.size()] == 'a' ? "b" : "a") << a << "\n";
    }
}
