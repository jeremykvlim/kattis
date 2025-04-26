#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        string s;
        cin >> s;

        if (s.size() == 1 || s == "22") {
            cout << s << "\n";
            continue;
        }

        for (int i = 0, total = 0; i <= s.size(); i++) {
            if ((i == s.size() || s[i] > '2') && total & 2) s.insert(i, "2");
            else if ((i == s.size() || s[i] > '4') && total & 4) s.insert(i, "4");
            total += s[i] - '0';
        }

        int total = accumulate(s.begin(), s.end(), 0, [](auto t, auto c) { return t + c - '0'; });
        while (total & (total - 1)) {
            total += 8;
            s += "8";
        }

        cout << s << "\n";
    }
}
