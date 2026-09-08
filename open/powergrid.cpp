#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> C(n, vector<int>(m));
    for (auto &row : C)
        for (int &cij : row) cin >> cij;

    vector<vector<int>> A;
    auto reconstruct = [&]() {
        int n = C.size(), m = C[0].size(), r = 0, c = 0;
        A.assign(n, vector<int>(m));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                if (C[i][j] > C[r][c]) {
                    r = i;
                    c = j;
                }

        int delta = -C[r][c];
        for (int j = 0; j < m; j++) delta += A[r][j] = C[r][c] - C[r][j];

        int total = 0;
        vector<pair<int, int>> changes;
        for (int i = 0; i < n; i++)
            if (i != r) {
                int d = C[i][c];
                bool pos = true, neg = true;
                for (int j = 0; j < m; j++) {
                    pos &= abs(A[r][j] - d) == C[i][j];
                    neg &= abs(A[r][j] + d) == C[i][j];
                }
                if (!pos && !neg) return false;

                delta -= A[i][c] = neg ? -d : d;
                if (pos && neg && d) {
                    changes.emplace_back(i, 2 * d);
                    total += 2 * d;
                }
            }

        vector<int> indices(total + 1, -1);
        dynamic_bitset<> dp(total + 1);
        dp[0] = 1;
        for (int i = 0; i < changes.size(); i++) {
            auto temp = (dp << changes[i].second) & ~dp;
            for (int d = temp.find_first(); d < temp.size(); d = temp.find_next(d)) indices[d] = i;
            dp |= temp;
        }

        if (n != m) {
            int d = abs(n - m);
            for (delta = (delta % d + d) % d; delta <= total && !dp[delta]; delta += d);
        }
        if (!(0 <= delta && delta <= total) || !dp[delta]) return false;

        while (delta) {
            auto [i, d] = changes[indices[delta]];
            A[i][c] += d;
            delta -= d;
        }

        int row_sum = C[r][c], col_sum = accumulate(A[r].begin(), A[r].end(), 0);
        for (int i = 0; i < n; i++)
            if (i != r) row_sum += A[i][c];

        for (int i = 0, add = n == m ? 0 : (col_sum - row_sum) / (n - m); i < max(n, m); i++) {
            if (i < m) A[r][i] += add;
            if (i < n && i != r) {
                A[i][c] += add;
                A[r][c] -= A[i][c];
            }
        }
        return true;
    };

    if (!reconstruct()) {
        vector<vector<int>> transpose(m, vector<int>(n));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) transpose[i][j] = C[j][i];

        C = transpose;
        reconstruct();
        transpose.assign(n, vector<int>(m));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) transpose[i][j] = A[j][i];
        A = transpose;
    }

    for (auto &row : A) {
        for (int aij : row) cout << aij << " ";
        cout << "\n";
    }
}
