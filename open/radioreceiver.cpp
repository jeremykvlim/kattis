#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int c;
    cin >> c;

    for (int x = 1; x <= c; x++) {
        int n;
        cin >> n;

        vector<int> p(n), t(n);
        for (int i = 0; i < n; i++) cin >> p[i] >> t[i];

        int d = 0;
        for (int i = 0; i < n; i++) 
            for (int j = 0; j < i; j++) d = max(d, abs(p[i] - p[j]) - abs(t[i] - t[j]));
        
        cout << "Case #" << x << ": " << fixed << setprecision(9) << d / 2.0 << "\n";
    }
}
