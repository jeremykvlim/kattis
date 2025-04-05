#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, m;
    cin >> N >> m;

    vector<vector<long long>> C(N + 1, vector<long long>(N + 1));
    for (int n = 0; n <= N; n++) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; k++) C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
    }

    vector<__float128> p(m - N + 1, 1), P(N, 0);
    for (int i = 1; i <= m - N; i++) p[i] = p[i - 1] * (m - N - i + 1) / (m - i + 1);
    for (int i = 1; i <= N; i++)
        for (int j = 1; i * (j - 1) <= m - N; j++) P[i - 1] += p[i * (j - 1)];

    vector<__float128> E(N, 0);
    for (int i = 0; i < N; i++)
        for (int k = i; k < N; k++) E[i] += ((k - i) & 1 ? -1 : 1) * C[N][k + 1] * P[k] * C[k][i];

    reverse(E.begin(), E.end());
    for (int i = 1; i < N; i++) E[i] += E[i - 1];
    for (auto Ei : E) cout << fixed << setprecision(6) << (double) Ei << "\n";
}
