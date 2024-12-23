#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int l1, a1, l2, a2, lt, at;
        cin >> l1 >> a1 >> l2 >> a2 >> lt >> at;

        int count = 0, r1, r2;
        for (int i = 1; i * l1 <= lt; i++) {
            if ((lt - i * l1) % l2 || lt == i * l1) continue;

            int j = (lt - i * l1) / l2;
            if (at - i * a1 == j * a2) {
                count++;
                r1 = i;
                r2 = j;
            }
        }
        if (count == 1) cout << r1 << " " << r2 << "\n";
        else cout << "?\n";
    }
}
