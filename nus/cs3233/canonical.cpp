#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> c(n);
    for (int &ci : c) cin >> ci;
    reverse(c.begin(), c.end());

    for (int i = 0; i < n - 1; i++) {
        int ci = c[i] - 1;
        for (int j = i + 1; j < n; j++) {
            ci %= c[j];

            int temp = c[i] - 1 - ci;
            auto coins = [&](int diff) {
                int count = 0;
                for (int ci : c) {
                    if (!diff) break;
                    count += diff / ci;
                    diff %= ci;
                }
                return count;
            };

            if (coins(temp) < coins(temp + c[j]) - 1) {
                cout << "non-canonical";
                exit(0);
            }
        }
    }
    cout << "canonical";
}
