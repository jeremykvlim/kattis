#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l1, l2, n;
    cin >> l1 >> l2 >> n;

    vector<int> tubes(n);
    for (int &t : tubes) cin >> t;
    sort(tubes.begin(),tubes.end());

    int air = -1;
    for (int i = n - 1, j = 0; j < n; j++) {
        while (~i && tubes[j] + tubes[i] > l1) i--;
        if (i < 0) break;

        for (int k = i; ~k && k > i - 4; k--) {
            if (j == k) continue;

            for (int l = 0, m = n - 1; l < n; l++) {
                if (l == j || l == k) continue;

                while (~m && (tubes[l] + tubes[m] > l2 || m == j || m == k || m == l)) m--;
                if (m < 0) break;

                air = max(air, tubes[j] + tubes[k] + tubes[l] + tubes[m]);
            }
        }
    }

    cout << (air == -1 ? "Impossible" : to_string(air));
}
