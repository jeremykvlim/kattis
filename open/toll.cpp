#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct DisjointSparseTable {
    vector<vector<T>> ST;
    function<T(T, T)> f;

    DisjointSparseTable() {}
    DisjointSparseTable(const vector<T> &v, function<T(T, T)> func) : f(func) {
        if (v.empty()) return;
        ST.resize(__lg(v.size()) + 1);
        ST[0] = v;
        for (int i = 1; i < ST.size(); i++) {
            ST[i].resize(v.size());
            for (int j = 0; j < v.size(); j += (1 << i) << 1) {
                int l = min(j + (1 << i), (int) v.size());
                ST[i][l - 1] = v[l - 1];
                for (int k = l - 1; k > j; k--) ST[i][k - 1] = f(v[k - 1], ST[i][k]);
                if (l >= v.size()) break;
                int r = min(l + (1 << i), (int) v.size());
                ST[i][l] = v[l];
                for (int k = l + 1; k < r; k++) ST[i][k] = f(ST[i][k - 1], v[k]);
            }
        }
    }

    T range_query(int l, int r) {
        if (l + 1 == r--) return ST[0][l];
        int i = __lg(l ^ r);
        return f(ST[i][l], ST[i][r]);
    }
};

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = numeric_limits<T>::max() / 4) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    friend auto operator*(const Matrix<T> &A, const Matrix<T> &B) {
        int r1 = A.r, c1 = A.c, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int k = 0; k < c1; k++)
                if (A[i][k] != numeric_limits<T>::max() / 4)
                    for (int j = 0; j < c2; j++) C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
        return C;
    }

    friend auto operator*=(Matrix<T> &A, const Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k, n, m, o;
    cin >> k >> n >> m >> o;

    int blocks = (n + k - 1) / k;
    vector<Matrix<int>> matrices(blocks - 1, Matrix<int>(k, k));
    while (m--) {
        int a, b, t;
        cin >> a >> b >> t;

        int qa = a / k, ra = a % k, rb = b % k;
        matrices[qa][ra][rb] = min(matrices[qa][ra][rb], t);
    }

    DisjointSparseTable<Matrix<int>> dst(matrices, [](const Matrix<int> &A, const Matrix<int> &B) { return A * B; });
    while (o--) {
        int a, b;
        cin >> a >> b;

        if (matrices.empty()) {
            cout << "-1\n";
            continue;
        }    
        
        int l = a / k, r = b / k;
        if (l == r) {
            cout << ((a == b) - 1) << "\n";
            continue;
        }

        int toll = dst.range_query(l, r)[a % k][b % k];
        cout << (toll == numeric_limits<int>::max() / 4 ? -1 : toll) << "\n";
    }
}
