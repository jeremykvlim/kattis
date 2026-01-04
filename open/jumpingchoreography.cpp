#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    int max_pos = t;
    vector<int> p(n);
    for (int &pi : p) {
        cin >> pi;

        max_pos = max(max_pos, pi);
    }

    int C;
    cin >> C;

    int ops = 0;
    vector<pair<char, int>> queries(C);
    for (auto &[c, a] : queries) {
        cin >> c >> a;

        max_pos = max(max_pos, a);
        ops += c != 't';
    }

    vector<int> lazy(max_pos + 3, 0), min_jumps(max_pos + 1, 0), total_jumps(max_pos + 1, 0);
    auto update = [&](int p, int sign) {
        lazy[p + 1] += sign;
        lazy[p + 2] += sign * 3;

        for (int r = p + 3, i = 1; r <= max_pos; r += ++i) lazy[r] += sign * ((i & 1) ? 1 : 3);

        lazy[0] += sign * min_jumps[p];
        if (p) {
            lazy[1] += sign * min_jumps[p - 1];
            lazy[p + 1] -= sign;
        }

        for (int l = p, i = 0; l > 1; l -= ++i) lazy[l] -= sign * ((i & 1) ? 1 : 3);
    };

    auto propagate = [&](vector<int> &v) {
        for (int i = 0; i <= max_pos; i++) {
            if (i > 1) lazy[i] += lazy[i - 2];
            v[i] += lazy[i];
        }
        fill(lazy.begin(), lazy.end(), 0);
    };

    update(0, 1);
    propagate(min_jumps);
    for (int pi : p) update(pi, 1);
    propagate(total_jumps);

    int size = ceil(sqrt(ops + 1)), sa = 0, sr = 0;
    vector<int> block_add(size), block_remove(size);
    for (auto [c, a] : queries) {
        if (c == 't') t = a;
        else if (c == '+') block_add[sa++] = a;
        else block_remove[sr++] = a;

        if (sa == size || sr == size) {
            for (int i = 0; i < sa; i++) update(block_add[i], 1);
            sa = 0;

            for (int i = 0; i < sr; i++) update(block_remove[i], -1);
            sr = 0;

            propagate(total_jumps);
        }

        int jumps = total_jumps[t];
        for (int i = 0; i < sa; i++) jumps += min_jumps[abs(block_add[i] - t)];
        for (int i = 0; i < sr; i++) jumps -= min_jumps[abs(block_remove[i] - t)];
        cout << jumps << "\n";
    }
}
