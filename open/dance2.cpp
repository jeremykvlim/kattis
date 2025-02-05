#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> count_l(251, 0), count_f(251, 0);
    while (n--) {
        int l, f;
        cin >> l >> f;

        count_l[l]++;
        count_f[f]++;

        int diff = 0;
        for (int i = 50, j = 50, cl = count_l[i], cf = count_f[j];; diff = max(diff, abs(i - j))) {
            while (i <= 250 && !cl)
                if (++i <= 250) cl = count_l[i];
            if (i > 250) break;

            while (j <= 250 && !cf)
                if (++j <= 250) cf = count_f[j];
            if (j > 250) break;

            int temp = min(cl, cf);
            cl -= temp;
            cf -= temp;
        }
        cout << diff << "\n";
    }
}
