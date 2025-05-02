#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, q;
    cin >> n >> k >> q;

    int i = -1;
    bool w = false;
    vector<int> flavours(n), count(k, 0);
    vector<bool> winning(2 * n - 1, false);
    for (int j = -1, m = 0; j < n;) {
        if (k) {
            if (++j == n) break;

            int f;
            cin >> f;

            flavours[j] = f;
            if (!count[f]++) k--;
            if (i >= 0) winning[i + j] = w = !w;
        } else {
            if (++i == n) break;

            winning[i + j] = w;
            if (!--count[flavours[i]]) {
                k++;
                if ((m - i) & 1) winning[j + m] = w = true;
                else w = false;

                for (; m < i; m++) winning[j + m + 1] = w = !w;
            }
            flavours[i] = j;
        }
    }
    fill(flavours.begin() + i + 1, flavours.end(), n);

    while (q--) {
        int l, r;
        cin >> l >> r;
        l--;
        r--;

        cout << (flavours[l] > r ? 0 : (winning[l + r] ? 1 : 2)) << "\n";
    }
}
