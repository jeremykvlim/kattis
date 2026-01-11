#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, int v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
    Matrix(const vector<vector<T>> &mat) : r(mat.size()), c(mat[0].size()), mat(mat) {}

    friend auto operator*(const Matrix<T> &A, const Matrix<T> &B) {
        int r1 = A.r, c1 = A.c, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int k = 0; k < c1; k++)
                if (A[i][k])
                    for (int j = 0; j < c2; j++) C[i][j] += A[i][k] * B[k][j];
        return C;
    }

    friend auto operator*=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }
};

template <typename T>
Matrix<T> I(int r, int c) {
    Matrix<T> I(r, c);
    for (int i = 0; i < min(r, c); i++) I[i][i] = 1;
    return I;
}

struct PURQSegmentTree {
    static inline vector<int> a;
    static inline int size;

    struct Monoid {
        Matrix<unsigned long long> M;

        Monoid() : M(I<unsigned long long>(11, 11)) {}

        auto & operator=(int b) {
            *this = Monoid();
            int b_l = b * size + 1, b_r = min((int) a.size() - 1, (b + 1) * size);
            vector<unsigned long long> sum(11, 1);
            sum[10] = 0;
            for (int i = b_l; i <= b_r; i++) {
                auto &row = M.mat[a[i]];
                for (int c = 0; c < 10; c++) {
                    auto temp = row[c];
                    row[c] = sum[c];
                    sum[c] = 2 * sum[c] - temp;
                }
                auto temp = row[10];
                row[10] = sum[10] + 1;
                sum[10] = 2 * sum[10] - temp + 1;
            }
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            M = monoid.M * M;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, const vector<int> &arr, int s) : n(n), ST(2 * n) {
        a = arr;
        size = s;
        for (int i = 0; i < n; i++) ST[i + n] = i;
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    int size = ceil(sqrt(n)), blocks = (n + size - 1) / size;
    PURQSegmentTree st(bit_ceil((unsigned) blocks), a, size);

    auto plans = [&]() {
        auto sum = 1ULL;
        for (int r = 0; r < 10; r++) sum += st[1].M[r][10];
        return sum;
    };
    cout << plans() << "\n";
    while (q--) {
        int i, v;
        cin >> i >> v;

        if (st.a[i] != v) {
            st.a[i] = v;
            int b = (i - 1) / size;
            st.point_update(b, b);
        }
        cout << plans() << "\n";
    }
}
