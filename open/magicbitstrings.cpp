#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p;
    while (cin >> p && p) {
        if (p == 2) {
            cout << "Impossible\n";
            continue;
        }

        string magic(p - 1, '1');
        for (auto i = 1LL; i < p; i++) magic[((i * i) % p) - 1] = '0';
        cout << magic << "\n";
    }
}
