#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        double a, b, t;
        cin >> a >> b >> t;
        auto A = llround(a * 100), B = llround(b * 100), T = llround(t * 100);

        auto atp = [&](auto x) {return (x * (10000 + T) + 5000) / 10000;};
        auto search = [&](auto p) {
            long long l = -1, r = p + 1, m, value;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (atp(m) <= p) l = value = m;
                else r = m;
            }

            return value;
        };

        long long C = search(A), D = search(B);
        if (atp(C) != A) D--;
        cout << (B - A) - (D - C) << "\n";
    }
}
