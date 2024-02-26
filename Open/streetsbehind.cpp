#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long n, k, a, b;
        cin >> n >> k >> a >> b;

        auto runs = 0LL;
        while (k) {
            auto c = n * (b - a) / a;
            if (!c) {
                runs = -1;
                break;
            }

            auto r = min(((c + 1) * a - n * (b - a) - 1) / (c * (b - a)), (k - 1) / c) + 1, convert = min(r * c, k);
            runs += r;
            n += convert;
            k -= convert;
        }

        cout << runs << "\n";
    }
}
