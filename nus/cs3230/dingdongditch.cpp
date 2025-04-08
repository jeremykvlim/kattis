#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    
    vector<int> a(n);
    for (int &ai : a) cin >> ai;
    sort(a.begin(), a.end());
    
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) pref[i] = pref[i - 1] + a[i - 1];
    
    while (q--) {
        int b;
        cin >> b;
        cout << pref[b] << "\n";
    }
}
