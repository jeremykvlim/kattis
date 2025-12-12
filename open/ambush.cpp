#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, a, b, p;
    cin >> l >> a >> b >> p;
    if (a > b) swap(a, b);

    int tl = b - 1, tr = l - a - 1;
    if (p < a) cout << tl;
    else if (p > b) cout << tr;
    else {
        int gap = b - a;
        if (gap == 2) cout << min(tl, tr) + 1;
        else if (p == a + 1) {
            if (tl <= tr) cout << tl + !(gap & 1);
            else cout << tl;
        } else if (p == b - 1) {
            if (tl >= tr) cout << tr + !(gap & 1);
            else cout << tr;
        } else if (tl != tr) cout << max(tl, tr);
        else cout << tl + !(gap & 1);
    }
}
