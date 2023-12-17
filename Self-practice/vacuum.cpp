#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l1, l2, n;
    cin >> l1 >> l2 >> n;

    vector<int> tubes(n);
    for (int &l : tubes) cin >> l;
    sort(tubes.begin(),tubes.end());

    int air = -1, x = n - 1;
    for (int i = 0; i < n; i++) {
        while (x >= 0 && tubes[i] + tubes[x] > l1) x--;
        if (x < 0) break;

        for (int j = x, y = n - 1; j > x - 4 && j >= 0; j--) {
            if (i == j) continue;

            for (int k = 0; k < n; k++) {
                if (k == i || k == j) continue;
                while (y >= 0 && (tubes[k] + tubes[y] > l2 || y == i || y == j || y == k)) y--;
                if (y < 0) break;

                air = max(air, tubes[i] + tubes[j] + tubes[k] + tubes[y]);
            }
        }
    }

    if (air >= 0) cout << air;
    else cout << "Impossible";
}
