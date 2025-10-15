#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r;
    cin >> n >> r;

    auto mask = ((__int128) 1 << r) - 1;
    vector<__int128> masks(2 * n);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < 2 * n; j++) {
            int x;
            cin >> x;

            if (x > n) masks[j] |= (__int128) 1 << i;
        }
    sort(masks.begin(), masks.end());

    for (int i = 0; i < n; i++)
        if ((masks[i] ^ masks[2 * n - i - 1]) != mask) {
            cout << "impossible";
            exit(0);
        }
    cout << "possible";
}
