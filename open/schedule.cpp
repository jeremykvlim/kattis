#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w;
    cin >> n >> w;

    for (int iso = 4; iso <= w; iso++) {
        vector<int> teams;
        for (int mask = 0; mask < 1 << (iso - 1) && teams.size() < n; mask++)
            if (__builtin_popcount(mask) == (iso - 1) / 2 + 1) teams.emplace_back(mask);

        if (teams.size() == n) {
            cout << iso << "\n";
            for (int k = 0; k < w; k++) {
                if (!(k % iso)) cout << string(n, '1') << "\n";
                else {
                    for (int i : teams) cout << (char) ('1' + ((i >> (k % iso - 1)) & 1));
                    cout << "\n";
                }
            }
            exit(0);
        }
    }

    cout << "infinity";
}
