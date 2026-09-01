#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << (char) (j & 1 ? 'n' + min(i % 26, 25 - i % 26) : 'a' + i / 13);
        cout << "\n";
    }
}
