#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    
    vector<long long> sizes(2 * n), pref((2 * n) + 1, 0), pref2((2 * n) + 2, 0);
    for (int i = 0; i < 2 * n; i++) {
        cin >> sizes[i];
        pref[i + 1] = pref[i] + sizes[i];
        if (!(i & 1)) pref2[i + 2] = pref2[i] + sizes[i];
    }

    auto area = LLONG_MAX;
    for (int i = 0; i <= n; i++) area = min(area, (pref[i] + pref2[2 * n] - pref2[2 * i]) * (sizes[0] + sizes[i]));

    cout << area;
}
