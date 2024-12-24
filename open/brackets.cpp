#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<int> l(s.size() + 2, 0), r(s.size() + 2, 0);
    for (int i = 1; i <= s.size(); i++) {
        l[i] = l[i - 1] + (s[i - 1] == '(' ? 1 : -1);
        if (l[i] < 0) l[i] = -2;
    }

    for (int i = s.size(); i; i--) {
        r[i] = r[i + 1] + (s[i - 1] == ')' ? 1 : -1);
        if (r[i] < 0) r[i] = -2;
    }

    if (!l[s.size()]) {
        cout << "possible";
        exit(0);
    }

    for (int i = 0; i <= s.size(); i++)
        for (int j = i + 1; j <= s.size() && l[i] >= 0; j++) {
            l[i] += (s[j - 1] == '(' ? -1 : 1);
            if (l[i] == r[j + 1]) {
                cout << "possible";
                exit(0);
            }
        }

    cout << "impossible";
}
