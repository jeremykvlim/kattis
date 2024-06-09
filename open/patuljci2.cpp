#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<vector<int>> indices(c + 1);
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        indices[x].emplace_back(i);
    }

    int m;
    cin >> m;

    while (m--) {
        int a, b;
        cin >> a >> b;

        for (int x = 1; x <= c; x++)
            if ((lower_bound(indices[x].begin(), indices[x].end(), b) - lower_bound(indices[x].begin(), indices[x].end(), a - 1)) > (b - a + 1) / 2) {
                cout << "yes " << x << "\n";
                goto next;
            }

        cout << "no\n";
        next:;
    }
}
