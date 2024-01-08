#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> money(n);
        for (int &m : money) cin >> m;

        sort(money.begin(), money.end());
        for (int i = 0; i < n; i++) cout << money[(n - i) % n] << "-" << (i & 1 ? "B" : "A") << (i == n - 1 ? "\n" : " ");
    }
}
