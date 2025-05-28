#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> count(n + 1, 0);
    while (k--) {
        int x;
        cin >> x;

        count[x]++;
    }

    vector<int> freq(n + 1, 0);
    for (int x = 1; x < n; x++)
        if (count[x])
            for (int i = 0; i < n; i += x) freq[i] += count[x];

    vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + freq[i];

    int q;
    cin >> q;

    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << pref[r + 1] - pref[l] << "\n";
    }
}
