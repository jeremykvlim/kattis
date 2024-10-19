#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n + 1), s(n + 1), adj_p(n * 2 + 1, 0), adj_s(n * 2 + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> p[i] >> s[i];

        adj_p[p[i]] = s[i];
        adj_s[s[i]] = p[i];
    }

    for (int i = 2; i <= n * 2; i += 2) {
        if (!adj_s[i]) {
            cout << "YES\n";
            for (int j = 1; j <= n; j++) cout << j << " " << i / 2 << "\n";
            exit(0);
        }
        if (!adj_p[i]) {
            cout << "YES\n";
            for (int j = 1; j <= n; j++) cout << i / 2 << " " << j << "\n";
            exit(0);
        }
    }

    if (n == 1) {
        cout << "NO";
        exit(0);
    }

    cout << "YES\n";
    int pi = adj_s[2] / 2, pj = pi == 1 ? 2 : pi - 1, sj = adj_p[2 * pj] == 2 ? 2 : 1;
    for (int si = 2; si <= n; si++) cout << pi << " " << si << "\n";
    cout << pj << " " << sj;
}
