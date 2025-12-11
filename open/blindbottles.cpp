#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    iota(p.begin(), p.end(), 1);

    auto query = [&]() -> int {
        for (int pi : p) cout << pi << " ";
        cout << "\n" << flush;

        int k;
        cin >> k;

        if (k == n) exit(0);
        return k;
    };

    int base = query();
    for (int i = 0; i < n; i++) {
        int j0 = -1, j1 = -1, j2 = -1;
        for (int j = i + 1; j < n; j++) {
            swap(p[i], p[j]);
            int k = query();
            swap(p[i], p[j]);

            if (k == base + 1) {
                if (!~j0) j0 = j;
                else {
                    j1 = j;
                    break;
                }
            } else if (k == base + 2) {
                j2 = j;
                break;
            }
        }

        if (~j2) {
            swap(p[i], p[j2]);
            base += 2;
        } else if (~j0 && ~j1) {
            auto change = [&](int x, int y, int z) {
                p[i] = x;
                p[j0] = y;
                p[j1] = z;
            };
            int a = p[i], b = p[j0], c = p[j1];
            change(b, c, a);
            int k1 = query();
            change(a, b, c);
            change(c, a, b);
            int k2 = query();

            if (k1 >= k2) {
                change(b, c, a);
                base = k1;
            } else {
                change(c, a, b);
                base = k2;
            }
        }
    }
    query();
}
