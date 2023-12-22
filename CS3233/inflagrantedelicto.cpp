#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> prosecution(n + 1), defense(n);
    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        prosecution[p] = i;
    }
    for (int i = 0; i < n; i++) {
        int d;
        cin >> d;

        defense[i] = prosecution[d];
    }

    vector<int> lis(n, INT_MAX);
    for (int &d : defense) *lower_bound(lis.begin(), lis.end(), d) = d;

    cout << 2 << " " << lower_bound(lis.begin(), lis.end(), INT_MAX) - lis.begin() + 1;
}
