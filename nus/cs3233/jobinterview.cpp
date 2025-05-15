#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    static inline vector<long long> A, S;
    static inline int C, cand;

    static long long score(int l, int r) {
        return S[l] + (A[r] - A[l + 1]) * (r - l) + C;
    }

    struct Segment {
        int index, c;

        Segment() : index(-1), c(-1) {}

        auto & operator=(const int &v) {
            index = v;
            c = -1;
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            if (!~sl.index) seg.index = sr.index;
            else if (!~sr.index) seg.index = sl.index;
            else if (score(sl.index, cand) > score(sr.index, cand)) seg.index = sr.index;
            else {
                int l = cand, r = A.size(), m;
                while (l + 1 < r) {
                    m = l + (r - l) / 2;

                    if (score(sl.index, m) > score(sr.index, m)) r = m;
                    else l = m;
                }
                seg.index = sl.index;
                seg.c = r;
            }
            return seg;
        }
    };

    int n;
    vector<Segment> ST;
    vector<queue<int>> qs;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
        if (ST[i].c != -1) qs[ST[i].c].emplace(i);
    }

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void process(int c) {
        cand = c;
        while (!qs[c].empty()) {
            int i = qs[c].front();
            qs[c].pop();

            if (ST[i].c == c) assign(i - n, ST[i << 1 | 1].index);
        }
        S[c] = score(ST[1].index, c);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<long long> &a, int c) : n(n), ST(2 * n), qs(n) {
        cand = 0;
        A = a;
        S = vector<long long>(n, 0);
        C = c;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<long long> a(n + 1), k(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> k[i];

    int m = bit_ceil((unsigned) n);
    SegmentTree st(m, a, c);
    for (int i = 0; i < n; i++) {
        st.assign(i, i);
        if (i) {
            int b = ((st.S[i] + k[i]) % i) + 1;
            st.assign(b, st[b + m].index < 0 ? b : -1);
        }
        st.process(i + 1);
    }
    cout << st.S[n];
}
