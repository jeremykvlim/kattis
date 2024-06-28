#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;
    sort(h.begin(), h.end());

    int x = INT_MAX, i = -1;
    for (int l = 0, r = n / 2; r < n; l++, r++)
        if (x > h[r] - h[l]) {
            x = h[r] - h[l];
            i = l;
        }

    if (!(n & 1)) {
        cout << x;
        exit(0);
    }

    x = INT_MAX;
    vector<int> p(n);
    for (int j = 0, k = 1, l = i, r = (n / 2 + i + 1) % n; k < n; k++, l++, ++r %= n) {
        if (j < n) {
            p[j++] = h[l];
            if (j < n) p[j++] = h[r];
        }

        x = min(x, abs(p[k] - p[k - 1]));
    }

    cout << x;
}
