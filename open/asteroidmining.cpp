#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int A, Ainit, E;
    cin >> A >> Ainit >> E;

    int k = 300 * (E - A + Ainit) + 1;
    vector<vector<double>> m(A), suff(A);
    auto build = [&](int i, double extra) {
        int n = m[i].size();
        suff[i].resize(n + 1);
        suff[i][n] = extra;
        for (int j = n - 1; ~j; j--) suff[i][j] = suff[i][j + 1] + m[i][j];
    };

    for (int i = 0; i < Ainit; i++) {
        int n;
        cin >> n;

        m[i].resize(n);
        for (auto &mij : m[i]) cin >> mij;
        sort(m[i].rbegin(), m[i].rend());

        double extra = 0;
        for (int j = k; j < n; j++) extra += m[i][j];
        if (n > k) m[i].resize(k);
        build(i, extra);
    }

    vector<int> pos(A, 0);
    while (E--) {
        string e;
        cin >> e;

        if (e == "mine") {
            int a, g;
            cin >> a >> g;

            pos[a] += g;
        } else {
            int a1, a2;
            double f1, f2;
            cin >> a1 >> f1 >> a2 >> f2;

            int i = pos[a1], j = pos[a2], n = min(k, (int) (m[a1].size() + m[a2].size()) - i - j);
            while (n--)
                if (j == m[a2].size() || (i < m[a1].size() && m[a1][i] * f1 >= m[a2][j] * f2)) m[Ainit].emplace_back(m[a1][i++] * f1);
                else m[Ainit].emplace_back(m[a2][j++] * f2);
            build(Ainit++, suff[a1][i] * f1 + suff[a2][j] * f2);
        }
    }

    for (int i = 0; i < A; i++) cout << fixed << setprecision(6) << suff[i][pos[i]] << " " << (pos[i] < m[i].size() ? m[i][pos[i]] : 0) << "\n";
}