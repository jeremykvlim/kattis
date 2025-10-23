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
vector<int> rref(GaloisField2Matrix<S> &matrix) {
    int n = matrix.r, m = matrix.c;

    int rank = 0;
    vector<int> pivots;
    for (int c = m - 1; ~c && rank < n; c--) {
        int pivot = rank;
        for (; pivot < n && !matrix[pivot][c]; pivot++);

        if (pivot == n) continue;
        swap(matrix[pivot], matrix[rank]);
        pivots.emplace_back(rank);

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) matrix[i] ^= matrix[rank];

        rank++;
    }
    return pivots;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<bitset<64>> rows(n);
    for (auto &bs : rows) {
        long long a;
        cin >> a;

        bs = a;
    }

    GaloisField2Matrix<64> m(rows);
    auto xor_sum = 0ULL;
    for (int p : rref(m)) xor_sum = max(xor_sum, xor_sum ^ m[p].to_ullong());
    cout << xor_sum;
}
