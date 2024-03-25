#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    if (s.size() == 1 && !stoi(s)) {
        cout << 0;
        exit(0);
    }

    vector<int> l;
    for (char c : s) l.emplace_back(c - '0');

    int len = 0;
    for (int i = 0, j = l[i]; i < l.size(); i++, j = 10 * j + l[i]) {
        if (!len && j >= l.size() - i) len = l.size() - i;
        if (len) {
            l[i] = j / len;
            j %= len;
        }
    }

    for (int i = l.size() - len; i < l.size(); i++) cout << l[i];
}
