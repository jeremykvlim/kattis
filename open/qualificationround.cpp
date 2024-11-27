#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int p, c;
        cin >> p >> c;

        vector<long long> s(p);
        for (auto &si : s) cin >> si;
        sort(s.begin(), s.end());

        auto sum = 0LL;
        for (int i = 0; i < s.size() - c; i++) sum += s[i];

        auto y = s[s.size() - c];
        for (int i = 1, j = s.size() - c; i < c; i++) {
            auto diff = s[i + j] - s[i + j - 1];
            if (sum >= i * diff) {
                sum -= i * diff;
                y += diff;
            } else {
                y += sum / i;
                sum = 0;
                break;
            }
        }
        if (sum / c >= 0) y += sum / c;
        
        cout << "Case #" << x << ": " << y << "\n";
    }
}
