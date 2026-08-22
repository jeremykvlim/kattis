#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n = 0) : Matrix(n, n) {}
    Matrix(int rows, int cols, T v = 0) : r(rows), c(cols), mat(rows, vector<T>(cols, v)) {}
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

    friend auto operator*(const Matrix<T> &A, const vector<T> &v) {
        int n = A.r, m = v.size();

        vector<T> u(n, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) u[i] += A[i][j] * v[j];

        return u;
    }

    auto & operator[](int i) {
        return mat[i];
    }

    auto & operator[](int i) const {
        return mat[i];
    }

    Matrix<T> transpose() const {
        Matrix<T> mat_T(c, r);

        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) mat_T[j][i] = mat[i][j];

        return mat_T;
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 0;
    return I;
}

struct PURQSegmentTree {
    static inline vector<int> a;
    static inline int n, size;

    struct Monoid {
        static inline int p;
        static inline array<vector<unsigned long long>, 2> U, V;

        Matrix<unsigned long long> M;

        Monoid() : M(I<unsigned long long>(11)) {}
        Monoid(Monoid &&) = default;

        Monoid(const Monoid &monoid) {
            if (p <= 1) M = monoid.M;
            else if (p & 1) {
                auto M_T = monoid.M.transpose();
                V[0] = M_T * V[0];
                V[1] = M_T * V[1];
            }
        }

        auto & operator=(const Monoid &monoid) {
            if (p > 1) {
                p >>= 1;
                for (int i = 0; i <= 10; i++)
                    for (int j = 0; j <= 10; j++) M[i][j] += U[0][i] * V[0][j] - U[1][i] * V[1][j];
            } else M = monoid.M;

            return *this;
        }

        auto & operator=(int v) {
            if (p < 0) {
                for (int i = v * size + 1; i <= min((int) a.size() - 1, (v + 1) * size); i++) {
                    int r = a[i];
                    for (int c = 0; c <= 10; c++) {
                        auto temp = exchange(M[r][c], M[10][c]);
                        M[10][c] = 2 * M[10][c] - temp;
                    }
                }

                return *this;
            }

            int k = v / 10, b = (k - 1) / size;
            for (int i = 0; i < 2; i++) {
                fill(U[i].begin(), U[i].end(), 0);
                fill(V[i].begin(), V[i].end(), 0);
            }
            U[0][v % 10] = U[0][10] = U[1][a[k]] = U[1][10] = V[0][10] = V[1][10] = 1;
            V[0][v % 10] = V[1][a[k]] = -1;

            for (int i = k + 1; i <= min((int) a.size() - 1, (b + 1) * size); i++) {
                int c = a[i];
                for (int r = 0; r < 2; r++) {
                    auto temp = exchange(U[r][c], U[r][10]);
                    U[r][10] = 2 * U[r][10] - temp;
                }
            }

            for (int i = k - 1; i > b * size; i--) {
                int c = a[i];
                for (int r = 0; r < 2; r++) {
                    auto temp = V[r][c];
                    V[r][c] = -exchange(V[r][10], temp + 2 * V[r][10]);
                }
            }

            for (int i = 0; i <= 10; i++)
                for (int j = 0; j <= 10; j++) M[i][j] += U[0][i] * V[0][j] - U[1][i] * V[1][j];

            a[k] = v % 10;
            p = b + n;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (p > 1) {
                if (!(p & 1)) {
                    U[0] = monoid.M * U[0];
                    U[1] = monoid.M * U[1];
                }
                return *this;
            }

            M = monoid.M * M;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

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

    PURQSegmentTree(int m, const vector<int> &arr, int s) : ST(2 * m) {
        a = arr;
        n = m;
        size = s;

        for (int k = 0; k < 2; k++) {
            Monoid::U[k].resize(11);
            Monoid::V[k].resize(11);
        }
        Monoid::p = -1;
        for (int i = 0; i < m; i++) ST[i + m] = i;
        Monoid::p = 0;
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
    cout << st[1].M[10][10] << "\n";
    while (q--) {
        int i, v;
        cin >> i >> v;

        if (st.a[i] != v) st.point_update((i - 1) / size, 10 * i + v);
        cout << st[1].M[10][10] << "\n";
    }
}
