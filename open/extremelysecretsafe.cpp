#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int guesses = 270;
    auto guess = [&](int c, const auto &s) {
        guesses--;
        cout << c << " " << s << "\n" << flush;

        char r;
        cin >> r;

        if (r == '=') exit(0);

        return r;
    };

    string alnum{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"}, password(30, '0');
    auto search = [&](int c, int i) {
        auto s = password;
        fill(s.begin() + i, s.end(), 'z');

        int l = -1, r = alnum.size() - 1;
        while (l + 1 < r) {
            int m = l + (r - l) / 2;

            s[i] = alnum[m];

            if (guess(c, s) == '<') l = m;
            else r = m;
        }
        password[i] = alnum[r];
    };

    string s(30, 'z');
    int l = 0;
    while (l < 30) {
        int r = min(l + 5, 30);
        for (int i = l; i < r; i++) search(0, i);

        int g = guesses - 6 * (30 - l) - 1;
        while (g && guess(1, s) != '<') g--;
        if (!g) break;
        l = r;
    }

    for (int i = l; i < 30; i++) search(1, i);
    if (l == 30) guess(0, password);
    guess(1, password);
}
