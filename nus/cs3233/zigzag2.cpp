#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << 1;
        exit(0);
    }

    int k0;
    cin >> k0;
    n--;

    int k1;
    while (n--) {
        cin >> k1;

        if (k0 != k1) break;
    }

    if (!~n && k0 == k1) {
        cout << 1;
        exit(0);
    }

    int count = 2;
    while (n--) {
        int k2;
        cin >> k2;

        if ((k1 > k0 && k1 > k2) || (k1 < k0 && k1 < k2)) {
            count++;
            k0 = k1;
        }
        k1 = k2;
    }
    cout << count;
}
