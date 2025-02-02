#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<bitset<1001>> seq(n);
    for (int i = 0; i < n; i++) seq[i][i] = true;
    while (q--) {
        int o, x, y;
        cin >> o >> x >> y;
        x--;
        y--;

        if (!o) {
            seq[x] |= seq[y];
            for (int i = 0; i < n; i++)
                if (seq[i][x]) seq[i] |= seq[x];
        } else cout << (seq[x][y] ? "Jebb\n" : "Neibb\n");
    }
}
