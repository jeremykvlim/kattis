#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    void resize_columns(int columns) {
        c = columns;
        for (auto &row : mat) row.resize(c);
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
bool rref(Matrix<T> &matrix) {
    int n = matrix.r, m = matrix.c;

    vector<int> pivots(n, -1);
    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (fabs(matrix[i][c]) > fabs(matrix[pivot][c])) pivot = i;

        if (fabs(matrix[pivot][c]) < 1e-9) continue;
        swap(matrix[pivot], matrix[rank]);
        pivots[rank] = c;

        auto temp = 1 / matrix[rank][c];
        for (int j = 0; j < m; j++) matrix[rank][j] *= temp;

        for (int i = 0; i < n; i++)
            if (i != rank && fabs(matrix[i][c]) > 1e-9) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) matrix[i][j] -= temp * matrix[rank][j];
            }

        rank++;
    }

    for (int r = 0; r < rank; r++) {
        for (int c = pivots[r] + 1; c < m; c++)
            if (fabs(matrix[r][c]) > 1e-10) goto next;
        return true;
        next:;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    Matrix<double> A(n, 100 * n);
    unordered_map<string, int> atoms;
    for (int i = 0; i < n; i++) {
        string s;
        getline(cin, s);

        istringstream iss(s);
        vector<string> equation{istream_iterator<string>{iss}, istream_iterator<string>{}};

        int sgn1 = 1;
        for (int j = 0; j < equation.size(); j++) {
            auto t = equation[j];
            if (t == "=") {
                sgn1 = -1;
                continue;
            }

            if (all_of(t.begin(), t.end(), [&](char c) {return isalpha(c);})) {
                int sgn2 = 1;
                if (j && equation[j - 1] == "/") sgn2 = -1;
                A[i][atoms.count(t) ? atoms[t] : atoms[t] = atoms.size()] += sgn1 * sgn2;
            }
        }
    }
    A.resize_columns(atoms.size());
    cout << (rref(A) ? "invalid" : "valid");
}
