#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    for (int i = 0; i < 2 * c; i++) cout << (i & 1 ? "_" : " ");
    cout << "\n";
    for (int i = 0; i < r; i++) {
        for (int j = 0; j <= 2 * c; j++) {
            if (!j || j == 2 * c) cout << "|";
            else if (j & 1 && i == r - 1) cout << "_";
            else {
                if (!(j & 1)) cout << (r & 1 && i != r / 2 ? "|" : " ");
                else cout << (!(r & 1) && j / 2 != c / 2 ? "_" : " ");
            }
        }
        cout << "\n";
    }
}
