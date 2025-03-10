#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    int q;
    cin >> q;

    vector<int> pos;
    for (int i = 0, j = 1; j <= 1e6 + q; j++) {
        if (i < n && x[i] == j) i++;
        else pos.emplace_back(j);
    }

    while (q--) {
        int i;
        cin >> i;
        i--;

        swap(x[i], *upper_bound(pos.begin(), pos.end(), x[i]));
        cout << x[i] << "\n";
    }
}
