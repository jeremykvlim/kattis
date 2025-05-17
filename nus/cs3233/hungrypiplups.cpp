#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int s = 0;
    vector<int> A(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> A[i];

        s = max(s, A[i]);
    }

    vector<array<int, 4>> queries(m);
    for (auto &[t, l, r, x] : queries) {
        cin >> t;

        if (!t) {
            cin >> l >> x;
            s = max(s, x);
        } else cin >> l >> r >> x;
    }

    vector<tree<int, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>> pos(s + 1);
    for (int i = 1; i <= n; i++) pos[A[i]].insert(i);

    for (auto [t, l, r, x] : queries)
        if (!t) {
            if (A[l] != x) {
                pos[A[l]].erase(l);
                pos[x].insert(l);
                A[l] = x;
            }
        } else {
            int prev = 0;
            for (int X = 1; X <= s; X++) {
                int count = pos[X].order_of_key(r + 1) - pos[X].order_of_key(l);
                if (!count) continue;

                if (x <= max(X, prev + 1) - prev - 1) break;
                x -= max(X, prev + 1) - prev - 1;
                prev = max(X, prev + 1) + count - 1;
            }
            cout << x + prev << "\n";
        }
}
