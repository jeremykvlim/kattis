#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int n, m, t = 1; cin >> n >> m; t++) {
        vector<long long> a(n), b(m);
        for (auto &ai : a) cin >> ai;
        for (auto &bj : b) cin >> bj;
        sort(a.rbegin(), a.rend());
        sort(b.rbegin(), b.rend());

        auto A = a[0], B = b[0];
        for (int i = 1; i < max(n, m); i++) {
            if (i < n) A += a[i];
            if (A < B) break;
            if (i < m) B += b[i];
            if (A > B) break;
        }
      
        if (A > B) {
            cout << "Case " << t << ": " << "Takeover Incorporated\n";
            continue;
        }
        
        if (a[0] <= b[0]) {
            cout << "Case " << t << ": " << "Buyout Limited\n";
            continue;
        }
        
        A = a[0];
        B = b[1];
        for (int i = 1; i < max(n, m - 1); i++) {
            if (i + 1 < m) B += b[i + 1];
            if (A > B) break;
            if (i < n) A += a[i];
            if (A < B) break;
        }
        cout << "Case " << t << ": " << (A > B ? "Takeover Incorporated\n" : "Buyout Limited\n");
    }
}
