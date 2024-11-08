#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> A000071(27);
    A000071[0] = 1;
    A000071[1] = 2;
    for (int i = 2; i < 27; i++) A000071[i] = A000071[i - 1] + A000071[i - 2] + 1;

    int n, k;
    cin >> n >> k;

    int root = 0;
    vector<int> p(n + 1), l(n + 1, 0), r(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int pi;
        cin >> pi;

        if (pi == -1) root = i;
        else {
            p[i] = pi;
            (i < pi ? l[pi] : r[pi]) = i;
        }
    }

    vector<int> depth(n + 1, 0), deepest(n + 1, 0);
    auto build = [&](auto &&self, int v, int d = 0) {
        if (!v) return;

        self(self, l[v], d + 1);
        self(self, r[v], d + 1);
        depth[v] = d;
        deepest[v] = max({d, deepest[l[v]], deepest[r[v]]});
    };
    build(build, root);

    vector<int> final_depth(n + 1, 0), limit(n + 1, -1);
    vector<bool> keep(n + 1, false);
    auto dfs = [&](auto &&self, int v, int d = 0) {
        if (!v) return;

        int cl = l[v], cr = r[v];
        if (~limit[v]) {
            keep[v] = true;
            final_depth[v] = max(final_depth[v], limit[v]);
            if (d < limit[v]) {
                if (d + 1 == limit[v]) limit[cl ? cl : cr] = limit[v];
                else {
                    limit[cl] = limit[cr] = limit[v];
                    (limit[v] > deepest[cl] ? limit[cl] : limit[cr])--;
                }
            }
        }

        self(self, cl, d + 1);

        if (!keep[v]) {
            int kept = 1;
            for (int i = v; p[i]; i = p[i]) {
                int pi = p[i];
                if (!keep[pi]) kept++;

                if (pi < i && max(depth[pi], final_depth[pi]) < d - 1) {
                    kept = k + 1;
                    break;
                }

                if (pi > i) {
                    int h = d - depth[i];
                    if (h && limit[r[pi]] < d - 1) kept += A000071[h - 1] - (!~limit[r[pi]] ? 0 : A000071[limit[r[pi]] - depth[i]]);
                }
            }

            if (k >= kept) {
                k -= kept;
                final_depth[v] = max(final_depth[v], d);
                for (int i = v; p[i]; i = p[i]) {
                    int pi = p[i];
                    final_depth[pi] = max(final_depth[pi], d);
                    keep[pi] = true;

                    if (pi > i && i != v) limit[r[pi]] = max(limit[r[pi]], d - 1);
                }
                keep[v] = true;
            }
        }

        if (keep[v]) self(self, cr, d + 1);
    };
    dfs(dfs, root);

    for (int i = 1; i <= n; i++) cout << keep[i];
}
