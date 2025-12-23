#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<int> a(n), count(3 * n - k + 1, 0);
        for (int &ai : a) {
            cin >> ai;

            count[ai]++;
        }

        int ops = 0;
        for (int i = 1, len = 0; i <= 3 * n - k; i++) {
            if (count[i] > k) {
                count[i + 1] += count[i] - 1;
                len++;
            } else len = 0;
            ops = max(ops, len);
        }
        cout << ops << "\n";
    }
}
