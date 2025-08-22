#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, y;
    cin >> n >> x >> y;

    vector<int> p(n), indices(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];

        indices[p[i]] = i;
    }

    vector<int> t(n);
    vector<string> tricks;
    int d = y - x, lsb = d & -d;
    auto xor_s = [&](auto &temp, int s) {
        for (int i = 0; i < n; i++) {
            p[i] ^= s;

            indices[p[i]] = i;
        }
        tricks.emplace_back("S " + to_string(s));

        for (int i = 0; i < n; i++) t[i] = temp[i ^ s];
        temp = t;
    };

    if (lsb != 1) {
        for (int i = 0; i < n; i++)
            if (p[i] % lsb != p[i % lsb] % lsb) {
                cout << -1;
                exit(0);
            }

        auto temp = indices;
        for (;;) {
            int mask = 0;
            while (!(temp[mask] & (lsb / 2))) mask++;
            if (mask >= lsb / 2) break;

            for (int b = __lg(n); ~b; b--)
                if ((mask >> b) & 1) xor_s(temp, 1 << b);

            int k = n - 1;
            for (int i = 0; i < n; i++) {
                p[i] = (p[i] + k) % n;

                indices[p[i]] = i;
            }
            tricks.emplace_back("K " + to_string(k));

            for (int i = 0; i < n; i++) t[i] = temp[(i + 1) % n];
            temp = t;
        }

        int mask = 0;
        while (temp[mask] % lsb) mask++;

        for (int b = __lg(n); ~b; b--)
            if ((mask >> b) & 1) xor_s(temp, 1 << b);

        for (int i = 0; i < lsb; i++)
            if (temp[i] % lsb != i) {
                cout << -1;
                exit(0);
            }
    }

    auto sub = [n](int a, int b) {
        int diff = (a - b) % n;
        if (diff < 0) diff += n;
        return diff;
    };

    vector<pair<int, int>> precomp(n, {-1, -1});
    for (int r = 0; r < n; r++)
        for (int s = 0; s < n; s++) {
            int diff = sub((((r ^ s) + d) % n) ^ s, r);
            if (precomp[diff] == make_pair(-1, -1)) precomp[diff] = {r, s};
        }

    auto rearrange = [&](int p1, int p2) {
        int diff = sub(p2, p1);
        if (precomp[diff] == make_pair(-1, -1)) {
            cout << -1;
            exit(0);
        }

        auto [r, s] = precomp[diff];
        int k1 = sub(r, p1),
            k2 = sub(x, r ^ s),
            k3 = sub(r ^ s, x),
            k4 = sub(p1, r);

        auto prev = [&](int z) {
            return sub(sub(z, k2) ^ s, k1);
        };
        int px = prev(x), py = prev(y);
        swap(p[indices[px]], p[indices[py]]);
        swap(indices[px], indices[py]);

        if (k1) tricks.emplace_back("K " + to_string(k1));
        if (s) tricks.emplace_back("S " + to_string(s));
        if (k2) tricks.emplace_back("K " + to_string(k2));
        tricks.emplace_back("W");
        if (k3) tricks.emplace_back("K " + to_string(k3));
        if (s) tricks.emplace_back("S " + to_string(s));
        if (k4) tricks.emplace_back("K " + to_string(k4));
        return;
    };

    auto valid = [&](int p1, int p2) {
        int diff = abs(p1 - p2);
        return (diff / lsb) & 1 && !(diff % lsb);
    };

    for (int o = 0; o < lsb; o++)
        for (int i = o; i < n; i += lsb)
            if (p[i] != i) {
                if (valid(p[i], i)) {
                    rearrange(p[i], i);
                    continue;
                }

                for (int j = o; j < n; j += lsb)
                    if (i != j)
                        if (valid(p[i], p[j])) {
                            int pi = p[i], pj = p[j];
                            rearrange(pi, pj);
                            rearrange(pj, i);
                            rearrange(pi, pj);
                            break;
                        }
            }

    cout << tricks.size() << "\n";
    for (auto &trick : tricks) cout << trick << "\n";
}
