#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> npf(2e6 + 1, 0);
    vector<bool> visited(2e6 + 1, false);
    visited[1] = true;
    for (int i = 1; i <= 2e6; i++)
        if (visited[i])
            for (int j = i; j <= 2e6; j += i) npf[j]++;
        else
            for (int j = i; j <= 2e6; j += i) visited[j] = true;

    int q;
    cin >> q;

    while (q--) {
        int i;
        cin >> i;

        cout << npf[i] << "\n";
    }
}
