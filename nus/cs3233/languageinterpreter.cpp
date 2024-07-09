#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Matrix {
    int r, c;
    vector<vector<T>> mat;

    Matrix(int n) : Matrix(n, n) {}
    Matrix(int row, int col, int v = 0) : r(row), c(col), mat(row, vector<T>(col, v)) {}

    friend auto operator *(Matrix<T> &A, Matrix<T> &B) {
        int r1 = A.r, r2 = B.r, c2 = B.c;

        Matrix<T> C(r1, c2);
        for (int i = 0; i < r1; i++)
            for (int j = 0; j < c2; j++)
                for (int k = 0; k < r2; k++) C[i][j] += A[i][k] * B[k][j];

        return C;
    }

    friend auto operator *=(Matrix<T> &A, Matrix<T> &B) {
        return A = A * B;
    }

    auto & operator[](int i) {
        return mat[i];
    }
};

template <typename T>
Matrix<T> I(int n) {
    Matrix<T> I(n);
    for (int i = 0; i < n; i++) I[i][i] = 1;

    return I;
}

template <typename T, typename U>
Matrix<T> matpow(Matrix<T> A, U exponent) {
    int n = A.r;
    auto B = I<T>(n);

    while (exponent) {
        if (exponent & 1) B = A * B;
        A = A * A;
        exponent >>= 1;
    }
    return B;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    Matrix<unsigned> r(33);
    for (int i = 0; i < k; i++) cin >> r[0][i];
    r[0][32] = 1;

    stack<pair<Matrix<unsigned>, unsigned>> st;
    auto temp = I<unsigned>(33);
    st.emplace(temp, 1);
    while (n--) {
        string s;
        cin >> s;

        if (s == "add") {
            int x, y, z;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> _ >> z;

            temp = I<unsigned>(33);
            temp[x][x]--;
            temp[y][x]++;
            temp[z][x]++;

            st.top().first *= temp;
        } else if (s == "addi") {
            int x, y;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> _ >> y >> _ >> c;

            temp = I<unsigned>(33);
            temp[x][x]--;
            temp[y][x]++;
            temp[32][x] += c;

            st.top().first *= temp;
        } else if (s == "move") {
            int x, y;
            char _;
            cin >> _ >> x >> _ >> _ >> y;

            temp = I<unsigned>(33);
            temp[x][x]--;
            temp[y][x]++;

            st.top().first *= temp;
        } else if (s == "li") {
            int x;
            unsigned c;
            char _;
            cin >> _ >> x >> _ >> c;

            temp = I<unsigned>(33);
            temp[x][x]--;
            temp[32][x] += c;

            st.top().first *= temp;
        } else if (s == "for") {
            unsigned c;
            cin >> c;

            st.emplace(I<unsigned>(33), c);
        } else {
            auto temp2 = matpow(st.top().first, st.top().second);

            st.pop();
            st.top().first *= temp2;
        }
    }

    r *= st.top().first;
    for (int i = 0; i < k; i++) cout << r[0][i] << " ";
}
