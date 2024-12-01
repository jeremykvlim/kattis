#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    while (N--) {
        string s, t;
        cin >> s >> t;

        bool alice;
        int n = s.size(), m = t.size(), diff = n - m, r = diff / 2;
        if (n == m) alice = s == t;
        else if (diff & 1) {
            alice = s.substr(r, m) == t;
            alice &= (s.substr(r + 1, m) == t);
        } else {
            alice = s.substr(r, m) == t;
            alice |= (s.substr(r + 1, m) == t);
            alice |= (s.substr(r - 1, m) == t);
        }

        cout << (alice ? "Alice\n" : "Bob\n");
    }
}
