#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> t(n);
    for (int i = 0; i < n; i++) {
        cin >> t[i];

        t[i] -= i;
    }
    sort(t.begin(), t.end());

    auto total = 0LL;
    for (int i = 0; i < n; i++) total += abs(t[i] - t[n / 2]);
    cout << total;
}
