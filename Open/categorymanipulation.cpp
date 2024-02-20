#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;

    while (tc--) {
        int n;
        cin >> n;

        vector<int> r(n), t(n), s(n);
        for (int i = 0; i < n; i++) cin >> r[i] >> t[i] >> s[i];

        bool collinear = true;
        vector<bool> top(n, false);
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                for (int k = j + 1; k < n; k++) {
                    int a = (t[i] - t[k]) * (s[j] - s[k]) - (s[i] - s[k]) * (t[j] - t[k]),
                        b = (s[i] - s[k]) * (r[j] - r[k]) - (r[i] - r[k]) * (s[j] - s[k]),
                        c = (r[i] - r[k]) * (t[j] - t[k]) - (t[i] - t[k]) * (r[j] - r[k]);

                    if (a || b || c) {
                        collinear = false;
                        for (int sign : {-1, 1}) {
                            int u = INT_MIN;
                            for (int l = 0; l < n; l++) u = max(u, sign * a * r[l] + sign * b * t[l] + sign * c * s[l]);
                            for (int l = 0; l < n; l++)
                                if (u == sign * a * r[l] + sign * b * t[l] + sign * c * s[l]) top[l] = true;
                        }
                    }
                }

        for (bool b : top) cout << ((b ^ collinear) ? 'T' : 'F');
        cout << "\n";
    }
}
