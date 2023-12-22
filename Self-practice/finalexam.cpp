#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int na, nb, nc;
    cin >> na >> nb >> nc;

    vector<int> A(3e7 + 1, 0), B(3e7 + 1, 0), C(3e7 + 1, 0);
    int biggest = -1;
    while (na--) {
        int a;
        cin >> a;

        A[a]++;
        biggest = max(a, biggest);
    }

    while (nb--) {
        int b;
        cin >> b;

        B[b]++;
        biggest = max(b, biggest);
    }

    while (nc--) {
        int c;
        cin >> c;

        C[c]++;
        biggest = max(c, biggest);
    }

    vector<vector<int>> g(sqrt(biggest) + 1, vector<int>(sqrt(biggest) + 1, 0));
    for (int i = 1; i < g.size(); i++)
        for (int j = 1; j < i; j++) g[i][j] = i % j ? g[j][i % j] : j;

    long long triplets = 0;
    for (int m = 1; m * m <= biggest; m++)
        for (int n = 1; n < m && m * m + n * n <= biggest; n++) {
            if (g[m][n] > 1 || (m & 1) == (n & 1)) continue;
            int a = m * m - n * n, b = 2 * m * n, c = m * m + n * n;
            for (int ka = a, kb = b, kc = c; kc <= biggest; ka += a, kb += b, kc += c) triplets += (long long) (A[ka] * B[kb] + B[ka] * A[kb]) * C[kc];
        }

    cout << triplets;
}
