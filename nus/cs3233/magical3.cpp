#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        if (n == 3) {
            cout << "4\n";
            continue;
        }

        int base = INT_MAX;
        for (int i = 1; i <= sqrt(n - 3); i++)
            if (!((n - 3) % i)) {
                if (n % i == 3) base = min(base, i);
                else if (n % ((n - 3) / i) == 3) base = min(base, (n - 3) / i);
            }

        if (base == INT_MAX) cout << "No such base\n";
        else cout << base << "\n";
    }
}
