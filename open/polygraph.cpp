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
    vector<int> match(m, -1);
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (; pivot < n && !matrix[pivot][c]; pivot++);

        if (pivot == n) continue;
        swap(matrix[pivot], matrix[rank]);
        match[c] = rank;

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) matrix[i] ^= matrix[rank];

        rank++;
    }

    return match;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 0; x < t; x++) {
        int n, m;
        cin >> n >> m;

        vector<bitset<501>> rows(m);
        for (auto &row : rows) {
            int i;
            char c;
            cin >> i >> c;
            i--;

            if (c == 'T' || c == 'L') {
                int j;
                cin >> j;
                j--;

                row.flip(i);
                row.flip(j);
                if (c == 'L') row.flip(500);
            } else {
                int j, k;
                cin >> j >> k;
                j--;
                k--;

                row.flip(i);
                row.flip(j);
                row.flip(k);
                if (c == 'S') row.flip(500);
            }
        }

        GaloisField2Matrix<501> matrix(rows);
        auto match = rref(matrix);

        bitset<501> mask;
        for (int c = 0; c < n; c++) mask[c] = !~match[c];

        string cities(n, '-');
        for (int i = 0; i < n; i++) {
            if (!~match[i] || (matrix[match[i]] & mask).any()) continue;
            cities[i] = matrix[match[i]][500] ? 'T' : 'L';
        }

        cout << "Case #" << x + 1 << ": ";
        for (char c : cities) cout << c << " ";
        cout << "\n";
    }
}
