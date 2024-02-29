#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(2 * n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        a[i + n] = a[i];
    }

    int AND = 0;
    for (int i = 30; ~i; i--) {
        AND |= 1 << i;

        int count = 0, curr = 0;
        for (int j = 0; j < 2 * n; j++) {
            curr |= a[j];

            if ((AND & curr) == AND) {
                count++;
                curr = 0;
            }
        }

        if (count < 2 * k - 1) AND ^= 1 << i;
    }

    cout << AND;
}
