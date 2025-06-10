#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, i, t;
    cin >> n >> i >> t;

    vector<int> l(n + 1), r(n + 1, -1);
    l[0] = r[0] = t;
    for (int k = 1; k <= n; k++) {
        int i_low, i_high, t_low, t_high;
        cin >> i_low >> i_high >> t_low >> t_high;

        for (int d = k - 1; ~d; d--)
            if (i_low <= i + d && i + d <= i_high) {
                int tl = max(t_low, l[d]), tr = min(t_high, r[d]);
                if (tl <= tr) {
                    r[d] = max(r[d], tr + 1);
                    if (l[d + 1] > r[d + 1]) {
                        l[d + 1] = tl;
                        r[d + 1] = tr;
                    } else {
                        l[d + 1] = min(l[d + 1], tl);
                        r[d + 1] = max(r[d + 1], tr);
                    }
                }
            }
    }
    
    int count = 0;
    for (int k = 1; k <= n; k++)
        if (l[k] <= r[k]) count = max(count, r[k] - t + k);
    cout << count;
}
