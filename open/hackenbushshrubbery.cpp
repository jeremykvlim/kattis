#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> e(n);
    for (int &ei : e) {
        cin >> ei;
        ei--;
    }

    vector<int> XOR(n, 0), len(n, 0);
    for (int i = n - 1; ~i; i--) {
        len[i] = XOR[i] + 1;
        if (e[i] != -2) XOR[e[i]] ^= len[i];
    }

    int grundy = 0;
    for (int i = 0; i < n; i++)
        if (e[i] == -2) grundy ^= len[i];
    cout << "*" << grundy;
}
