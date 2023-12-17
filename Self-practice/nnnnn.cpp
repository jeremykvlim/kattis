#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<int> L;
    for (char c : s) L.emplace_back(c - '0');

    int len = -1, remaining = 0;
    for (int i = 0; i < L.size(); i++) {
        remaining = 10 * remaining + L[i];
        if (len == -1 && remaining >= L.size() - i) len = L.size() - i;
        if (len > 0) {
            L[i] = remaining / len;
            remaining %= len;
        }
    }

    if (L == vector<int>{0}) cout << 0;
    else
        for (int i = L.size() - len; i < L.size(); i++) cout << L[i];
}
