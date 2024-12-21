#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    vector<int> p(n);
    for (int &pi : p) cin >> pi;

    vector<int> lazy(1e6 + 1, 0), min_jumps(1e6 + 1, 0), total_jumps(1e6 + 1, 0);
    auto update = [&](int p, int sign) {
        lazy[p + 1] += sign;
        lazy[p + 2] += sign * 3;

        for (int r = p + 3, i = 1; r <= 1e6; r += ++i) lazy[r] += sign * ((i & 1) ? 1 : 3);

        lazy[0] += sign * min_jumps[p];
        if (p) {
            lazy[1] += sign * min_jumps[p - 1];
            lazy[p + 1] -= sign;
        }

        for (int l = p, i = 0; l > 1; l -= ++i) lazy[l] -= sign * ((i & 1) ? 1 : 3);
    };

    auto propagate = [&](vector<int> &v) {
        for (int i = 0; i <= 1e6; i++) {
            if (i > 1) lazy[i] += lazy[i - 2];
            v[i] += lazy[i];
        }
        fill(lazy.begin(), lazy.end(), 0);
    };

    update(0, 1);
    propagate(min_jumps);
    for (int pi : p) update(pi, 1);
    propagate(total_jumps);

    int C;
    cin >> C;

    int size = ceil(sqrt(5e3)), sa = 0, sr = 0;
    vector<int> add(size), remove(size);
    auto push = [&]() {
        for (int i = 0; i < sa; i++) update(add[i], 1);
        add.clear();
        sa = 0;

        for (int i = 0; i < sr; i++) update(remove[i], -1);
        remove.clear();
        sr = 0;

        propagate(total_jumps);
    };

    while (C--) {
        char c;
        int a;
        cin >> c >> a;

        if (c == 't') t = a;
        else if (c == '+') add[sa++] = a;
        else remove[sr++] = a;

        if (sa == size || sr == size) push();

        int jumps = total_jumps[t];
        for (int i = 0; i < sa; i++) jumps += min_jumps[abs(add[i] - t)];
        for (int i = 0; i < sr; i++) jumps -= min_jumps[abs(remove[i] - t)];
        cout << jumps << "\n";
    }
}
