#include <bits/stdc++.h>
using namespace std;

template <int S>
struct GaloisField2Matrix {
    int r, c;
    vector<bitset<S>> mat;

    GaloisField2Matrix(int rows, bool v = false) : r(rows), c(S), mat(rows, v ? ~bitset<S>() : bitset<S>()) {}
    GaloisField2Matrix(const vector<bitset<S>> &mat) : r(mat.size()), c(S), mat(mat) {}

    auto & operator[](int i) {
        return mat[i];
    }
};

template <int S>
void rref(GaloisField2Matrix<S> &matrix) {
    int n = matrix.r, m = matrix.c, rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (; pivot < n && !matrix[pivot][c]; pivot++);

        if (pivot == n) continue;
        swap(matrix[pivot], matrix[rank]);

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) matrix[i] ^= matrix[rank];

        rank++;
    }
}

vector<vector<int>> masks(int C, int len) {
    if (C == 3) {
        if (len == 2) return {{1, 2}, {4, 8}, {16, 32}, {21, 42}, {27, 45}};
        return {{1, 2, 4}, {8, 16, 32}, {64, 128, 256}, {73, 146, 292}, {83, 166, 287}};
    }

    if (len == 2) return {{1, 2}, {4, 8}, {5, 10}, {6, 11}};
    return {{1, 2, 4}, {8, 16, 32}, {9, 18, 36}, {10, 20, 35}};
}

vector<string> encode(int C, int K, int N, string X) {
    vector<string> S(K, string(N, '0'));

    for (int pos = 0, len; pos < N; pos += len) {
        len = pos + 3 == N ? 3 : 2;
        auto M = masks(C, len);

        int bits = 0;
        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) bits |= (X[i * N + j + pos] - '0') << (i * len + j);

        for (int i = 0; i < K; i++)
            for (int j = 0; j < len; j++) S[i][j + pos] = '0' + (popcount((unsigned) (bits & M[i][j])) & 1);
    }

    return S;
}

string decode(int C, int K, int N, vector<string> Y, vector<int> I) {
    string X(C * N, '0');

    for (int pos = 0, len; pos < N; pos += len) {
        len = pos + 3 == N ? 3 : 2;
        auto M = masks(C, len);

        int d = C * len;
        GaloisField2Matrix<10> m(d);
        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) {
                m[i * len + j] = bitset<10>(M[I[i]][j]);
                m[i * len + j][d] = Y[i][pos + j] - '0';
            }
        rref(m);

        for (int i = 0; i < C; i++)
            for (int j = 0; j < len; j++) X[i * N + j + pos] = '0' + m[i * len + j][d];
    }

    return X;
}
