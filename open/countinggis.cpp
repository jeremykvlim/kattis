#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    cin >> n >> l;

    vector<int> g(l);
    vector<bool> seen(n + 1);
    for (int i = 0; i < l; i++) {
        cin >> g[i];

        if (i && g[i - 1] >= g[i]) {
            cout << 0;
            exit(0);
        }
        
        seen[g[i]] = true;
    }

    if (g.back() != n) {
        cout << 0;
        exit(0);
    }

    auto a = 1LL;
    int count = 0;
    for (int i = n; i; i--)
        if (!seen[i]) a = (a * (g.size() - (lower_bound(g.begin(), g.end(), i) - g.begin()) + count++)) % MODULO;

    cout << a;
}
