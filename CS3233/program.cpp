#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> jump(k), freq(n, 0);
    vector<long long> pref_sum(n + 1, 0);

    for (auto &j : jump) cin >> j;
    sort(jump.begin(), jump.end());

    for (int i = 0; i < k;) {
        int curr = jump[i], count = 0;
        for (; i < k && curr == jump[i]; i++) count++;
        for (int j = 0; j < n; j += curr) freq[j] += count;
    }

    for (int i = 0; i < n; i++) pref_sum[i + 1] = pref_sum[i] + freq[i];

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        cout << pref_sum[r + 1] - pref_sum[l] << "\n";
    }
}
