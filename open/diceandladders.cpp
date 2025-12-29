#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, int v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
vector<int> rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    vector<int> match(m, -1);
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabsl(matrix[i][c]) > fabsl(matrix[pivot][c])) pivot = i;

        if (fabsl(matrix[pivot][c]) < 1e-20) continue;
        swap(matrix[pivot], matrix[rank]);
        match[c] = rank;

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabsl(matrix[i][c]) > 1e-20) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    return match;
}

template <typename T>
T kitamasa(const vector<T> &c, const vector<T> &a, long long k) {
    int n = a.size();

    auto mul = [&](const vector<T> &x, const vector<T> &y) {
        vector<T> z(2 * n + 1);
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++) z[i + j] += x[i] * y[j];

        for (int i = 2 * n; i > n; i--)
            for (int j = 0; j < n; j++) z[i - j - 1] += z[i] * c[j];

        z.resize(n + 1);
        return z;
    };

    vector<T> base(n + 1, 0);
    base[1] = 1;
    auto pow = [&](vector<T> base, long long exponent) {
        vector<T> value(n + 1);
        value[0] = 1;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };
    auto value = pow(base, k + 1);

    T kth = 0;
    for (int i = 0; i < n; i++) kth += value[i + 1] * a[i];
    return kth;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, k;
    long double p;
    cin >> R >> C >> k >> p;

    int n = R * C;
    vector<int> ladders(n, -1);
    while (k--) {
        int s, e;
        cin >> s >> e;

        ladders[s - 1] = e - 1;
    }

    int m = n + 1;
    vector<vector<pair<int, __float128>>> adj_list(m);
    for (int v = 0; v < n; v++) {
        array<int, 6> cells;
        for (int i = 1; i <= 6; i++)
            if (v >= n - i) cells[i - 1] = n;
            else {
                int u = v + i;
                if (ladders[u] != -1) u = ladders[u];
                cells[i - 1] = u >= n - 1 ? n : u;
            }
        sort(cells.begin(), cells.end());

        for (int i = 0; i < 6;) {
            int j = i;
            while (j < 6 && cells[i] == cells[j]) j++;
            adj_list[v].emplace_back(cells[i], (j - i) / (__float128) 6);
            i = j;
        }
    }
    adj_list[n].emplace_back(n, 1);

    vector<vector<__float128>> dp(2 * m + 1, vector<__float128>(m, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= 2 * m; i++) {
        vector<__float128> temp(m);
        for (int v = 0; v < m; v++) {
            if (dp[i - 1][v])
                for (auto [u, w] : adj_list[v]) temp[u] += dp[i - 1][v] * w;
        }
        dp[i] = temp;
    }

    vector<__float128> c;
    for (int cols = 1; cols <= m; cols++) {
        Matrix<__float128> dp_T(m, cols + 1);
        for (int row = 0; row < m; row++)
            for (int col = 0; col <= cols; col++) dp_T[row][col] = dp[col][row];
        auto match = rref(dp_T);

        c.assign(cols, 0);
        for (int col = 0; col < cols; col++)
            if (match[col] != -1) c[col] = dp_T[match[col]][cols];

        for (int i = 0; i + cols <= 2 * m; i++)
            for (int v = 0; v < m; v++) {
                __float128 sum = 0;
                for (int col = 0; col < cols; col++) sum += c[col] * dp[i + col][v];

                if (fabsl(sum - dp[i + cols][v]) > 1e-20) goto next;
            }
        break;
        next:;
    }

    reverse(c.begin(), c.end());
    vector<__float128> a(c.size());
    for (int i = 0; i < a.size(); i++) a[i] = dp[i][n];

    int l = 0, r = 1e8, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        if (kitamasa(c, a, mid) + 1e-20 >= p) r = mid;
        else l = mid;
    }
    cout << r;
}
