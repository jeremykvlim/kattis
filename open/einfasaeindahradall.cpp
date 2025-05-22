#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> l(n), r(n), range(n), coeff(n, 1);
    int s = 1;
    for (int i = 0; i < n; i++) {
        cin >> l[i] >> r[i];

        s *= range[i] = r[i] - l[i] + 1;
        if (i) coeff[i] = coeff[i - 1] * range[i - 1];
    }

    int p;
    cin >> p;

    vector<int> freq(s, 0);
    while (p--) {
        int i = 0;
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;

            i += (x - l[j]) * coeff[j];
        }
        freq[i]++;
    }

    for (int d = 0; d < n; d++) {
        int len = coeff[d] * range[d];
        for (int i = 0; i < s; i += len)
            for (int j = coeff[d]; j < len; j++) freq[i + j] += freq[i + j - coeff[d]];
    }

    int q;
    cin >> q;

    vector<int> a_off(n), b_off(n);
    vector<bool> zero(n);
    while (q--) {
        bool none = false;
        for (int d = 0; d < n; d++) {
            int a, b;
            cin >> a >> b;

            if (none || b < l[d] || a > r[d]) {
                none = true;
                continue;
            }
            a = max(a, l[d]);
            b = min(b, r[d]);
            a_off[d] = (a - l[d] - 1) * coeff[d];
            b_off[d] = (b - l[d]) * coeff[d];
            zero[d] = a == l[d];
        }

        if (none) {
            cout << "0\n";
            continue;
        }

        bool add = false;
        int i = accumulate(b_off.begin(), b_off.end(), 0), count = freq[i];
        for (int mask = 1, z = 0, prev = 0; mask < 1 << n; mask++, add = !add) {
            int gray = mask ^ (mask >> 1), d = __lg(gray ^ prev);
            if ((gray >> d) & 1) {
                if (zero[d]) z++;
                i += a_off[d] - b_off[d];
            } else {
                if (zero[d]) z--;
                i += b_off[d] - a_off[d];
            }

            if (!z) {
                if (add) count += freq[i];
                else count -= freq[i];
            }
            prev = gray;
        }

        cout << count << "\n";
    }
}
