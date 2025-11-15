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

struct SegmentTree {
    static inline Matrix<unsigned long long> A;
    static inline vector<int> a;
    static inline int size;

    struct Segment {
        Matrix<unsigned long long> M;

        Segment() {}
        Segment(const Matrix<unsigned long long> &M) : M(M) {}

        auto & operator=(const int &b) {
            int il = b * size + 1, ir = min((int) a.size() - 1, (b + 1) * size);

            auto update = [&](int c = 0, unsigned long long s = 0) {
                vector<unsigned long long> count(10, 0);
                auto sum = count[c] = s;
                for (int i = il; i <= ir; i++) {
                    auto temp = count[a[i]];
                    count[a[i]] = sum + 1;
                    sum += count[a[i]] - temp;
                }
                return count;
            };
            auto temp = update();

            for (int c = 0; c < 10; c++) A[c] = update(c, 1);

            M = Matrix<unsigned long long>(11);
            for (int r = 0; r < 10; r++) {
                M[r][10] = temp[r];
                for (int c = 0; c < 10; c++) M[r][c] = A[c][r] - temp[r];
            }
            for (int c = 0; c < 10; c++) M[10][c] = 0;
            M[10][10] = 1;

            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            seg.M = sr.M * sl.M;
            return seg;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, int s, const vector<int> &arr) : n(n), ST(2 * n, I<unsigned long long>(11, 11)) {
        size = s;
        a = arr;
        A = Matrix<unsigned long long>(10);
        for (int i = 0; i < n; i++) assign(i, i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    int size = 1 << 7;
    SegmentTree st(bit_ceil((unsigned) n / size), size, a);

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
            st.assign(b, b);
        }
        cout << plans() << "\n";
    }
}
