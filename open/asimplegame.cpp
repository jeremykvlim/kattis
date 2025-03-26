#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, M, g;
    cin >> T >> M >> g;

    vector<vector<int>> A(T, vector<int>(M));
    for (auto &row : A)
        for (int &Aij : row) cin >> Aij;

    vector<int> tyko(T, 0), maj(M, 0);
    tyko[0] = maj[0] = 1;
    
    auto fictitious_play = [&]() {
        for (int i = 1, t = 0, m = 0; i < 250000; i++) {
            vector<double> p_tyko(T), p_maj(M);
            for (int r = 0; r < T; r++) p_tyko[r] = (double) tyko[r] / i;
            for (int c = 0; c < M; c++) p_maj[c] = (double) maj[c] / i;

            double s = -1e9;
            for (int r = 0; r < T; r++) {
                double sum = 0;
                for (int c = 0; c < M; c++) sum += A[r][c] * p_maj[c];

                if (s < sum) {
                    s = sum;
                    t = r;
                }
            }

            s = 1e9;
            for (int c = 0; c < M; c++) {
                double sum = 0;
                for (int r = 0; r < T; r++) sum += A[r][c] * p_tyko[r];

                if (s > sum) {
                    s = sum;
                    m = c;
                }
            }

            tyko[t]++;
            maj[m]++;
        }

        vector<double> equilibrium(T);
        for (int r = 0; r < T; r++) equilibrium[r] = (double) tyko[r] / 250000;
        return equilibrium;
    };
    auto equilibrium = fictitious_play();

    int remaining = 1e5;
    vector<int> count(T, 0);
    vector<pair<double, int>> exact(T);
    for (int r = 0; r < T; r++) {
        count[r] = equilibrium[r] * 1e5;
        remaining -= count[r];
        exact[r] = {equilibrium[r] * 1e5, r};
    }
    sort(exact.rbegin(), exact.rend());
    exact.resize(remaining);
    for (auto [d, r] : exact) count[r]++;

    for (int r = 0; r < T; r++)
        while (count[r]--) cout << r + 1 << " ";
}
