#include <bits/stdc++.h>
using namespace std;

template <typename T>
T held_karp(int n, const vector<vector<T>> &dist, int src = 0) {
    T inf = numeric_limits<T>::max() / 4;
    vector<vector<T>> dp(1 << n, vector<T>(n, inf));

    for (int i = 0; i < n; i++) dp[1 << i][i] = dist[src][i + 1];

    for (int m1 = 1; m1 < 1 << n; m1++)
        for (int i = 0; i < n; i++)
            if ((m1 >> i) & 1)
                if (dp[m1][i] != inf)
                    for (int m2 = m1 ^ ((1 << n) - 1); m2; m2 &= m2 - 1) {
                        int j = countr_zero((unsigned) m2), m3 = m1 | (1 << j);
                        dp[m3][j] = min(dp[m3][j], dp[m1][i] + dist[i + 1][j + 1]);
                    }

    T len = inf;
    for (int i = 0; i < n; i++) len = min(len, dp.back()[i] + dist[i + 1][src]);
    return len;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int x = 1; x <= n; x++) {
        int K;
        string s;
        cin >> K >> s;

        int blocks = s.size() / K;
        vector<vector<int>> inside(K, vector<int>(K, 0)), between(K, vector<int>(K, 0));
        for (int b = 0; b < blocks; b++)
            for (int i = 0; i < K; i++)
                for (int j = 0; j < K; j++) inside[i][j] += s[b * K + i] != s[b * K + j];

        for (int b = 0; b + 1 < blocks; b++)
            for (int i = 0; i < K; i++)
                for (int j = 0; j < K; j++) between[i][j] += s[b * K + i] != s[(b + 1) * K + j];

        int y = s.size();
        for (int k = 0; k < K; k++) {
            vector<int> p{k};
            for (int i = 0; i < K; i++)
                if (i != k) p.emplace_back(i);

            vector<vector<int>> dist(K, vector<int>(K));
            for (int i = 0; i < K; i++)
                for (int j = 0; j < K; j++) dist[i][j] = j ? inside[p[i]][p[j]] : between[p[i]][p[j]];
            y = min(y, held_karp(K - 1, dist) + 1);
        }
        cout << "Case #" << x << ": " << y << "\n";
    }
}