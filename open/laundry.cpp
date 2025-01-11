#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k;
        long long A, B, C, AB, BC, AC, ABC;
        cin >> k >> A >> B >> C >> AB >> BC >> AC >> ABC;

        ABC += A + B + C + AB + BC + AC;
        AB += A + B;
        BC += B + C;
        AC += A + C;

        A = (A / k) + !!(A % k);
        B = (B / k) + !!(B % k);
        C = (C / k) + !!(C % k);
        AB = (AB / k) + !!(AB % k);
        BC = (BC / k) + !!(BC % k);
        AC = (AC / k) + !!(AC % k);
        ABC = (ABC / k) + !!(ABC % k);

        AB = max(AB - A - B, 0LL);
        BC = max(BC - B - C, 0LL);
        AC = max(AC - A - C, 0LL);
        
        cout << max(A + B + C + max({AB, BC, AC, (AB + BC + AC + 1) / 2}), ABC) << "\n";
    }
}
