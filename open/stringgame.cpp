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

        int n = s.size(), m = t.size(), diff = n - m, r = diff / 2;
        auto same = [&](int r) {
            return s.substr(r, m) == t;
        };

        bool a;
        if (n == m) a = s == t;
        else if (diff & 1) a = same(r) & same(r + 1);
        else a = same(r) | same(r + 1) | same(r - 1);
        cout << (a ? "Alice\n" : "Bob\n");
    }
}
