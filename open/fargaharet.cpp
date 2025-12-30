struct PURQSegmentTree {
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

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Segment range_query(int l, int r) {
        Segment sl, sr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = sl + ST[l++];
            if (r & 1) sr = ST[--r] + sr;
        }

        return sl + sr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n, int s, const vector<int> &arr) : n(n), ST(2 * n, I<unsigned long long>(11, 11)) {
        size = s;
        a = arr;
        A = Matrix<unsigned long long>(10);
        for (int i = 0; i < n; i++) point_update(i, i);
    }
};