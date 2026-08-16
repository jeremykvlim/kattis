#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> order{1}, prev(n + 1);
    for (int i = 2; i <= n; i++) {
        int l = 0, r = order.size() + 1, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            cout << "? " << order[m - 1] << " " << i << "\n" << flush;

            char _;
            int k;
            cin >> _ >> k;

            if (k == order.size() - m + 1) l = m;
            else r = m;
        }

        if (l) {
            prev[i] = order[l - 1];
            order.erase(order.begin() + l - 1);
        }
        order.emplace_back(i);
    }

    cout << "! ";
    for (int i : order)
        for (int j = i; j; j = prev[j]) cout << j << " ";
}
