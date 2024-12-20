#include <bits/stdc++.h>
using namespace std;

vector<double> convolve(const vector<double> &a, const vector<double> &b) {
    int n = bit_ceil(a.size() + b.size() - 1);
    vector<int> rev(n, 0);
    for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] | (i & 1) << __lg(n)) >> 1;

    vector<complex<double>> twiddles(n, 1);
    for (int k = 2; k < n; k <<= 1) {
        auto w = polar(1., M_PI / k);
        for (int i = k; i < k << 1; i++) twiddles[i] = i & 1 ? twiddles[i >> 1] * w : twiddles[i >> 1];
    }

    auto fft = [&](vector<complex<double>> &v) {
        for (int i = 0; i < n; i++)
            if (i < rev[i]) swap(v[i], v[rev[i]]);

        for (int k = 1; k < n; k <<= 1)
            for (int i = 0; i < n; i += k << 1)
                for (int j = 0; j < k; j++) {
                    auto t = v[i + j + k] * twiddles[j + k];
                    v[i + j + k] = v[i + j] - t;
                    v[i + j] += t;
                }
    };
    vector<complex<double>> dft(n);
    for (int i = 0; i < a.size(); i++) dft[i].real(a[i]);
    for (int i = 0; i < b.size(); i++) dft[i].imag(b[i]);
    fft(dft);

    for (auto &d : dft) d *= d;
    fft(dft);
    reverse(dft.begin() + 1, dft.end());

    vector<double> c(a.size() + b.size() - 1, 0.5);
    for (int i = 0; i < c.size(); i++) c[i] *= dft[i].imag() / n;
    return c;
}

struct SegmentTree {
    struct Segment {
        vector<double> poly;
        int offset;

        Segment() : poly{1, 0}, offset(0) {}

        bool operator==(const Segment &seg) const {
            return poly == seg.poly && offset == seg.offset;
        }

        auto & operator=(double v) {
            poly = {1 - v, v};
            return *this;
        }

        auto operator+=(const Segment &seg) {
            auto p = convolve(poly, seg.poly);

            int l = find_if(p.begin(), p.end(), [](auto value) {return value > 1e-11;}) - p.begin(),
                r = find_if(p.rbegin(), p.rend(), [](auto value) {return value > 1e-11;}) - p.rbegin();

            poly = {p.begin() + l, p.end() - r};
            offset += seg.offset + l;
            return *this;
        }

        friend auto operator+(Segment sl, Segment sr) {
            if (sl == Segment()) return sr;
            if (sr == Segment()) return sl;
            return sl += sr;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l + 1);
        return min(l + i - 1, r - (i >> 1));
    }

    Segment range_query(int i, vector<int> &indices, int li, int ri, int l, int r) {
        if (li == ri) return ST[i];
        if (l == r) return {};

        int m = midpoint(l, r), mi = upper_bound(indices.begin() + li, indices.begin() + ri, m) - indices.begin();
        return range_query(i << 1, indices, li, mi, l, m) + range_query(i << 1 | 1, indices, mi, ri, m + 1, r);
    }

    Segment range_query(vector<int> &indices) {
        return range_query(1, indices, 0, indices.size(), 1, n);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, vector<double> &a) : n(n), ST(2 * n) {
        int m = bit_ceil(a.size());
        for (int i = 0; i < a.size(); i++) ST[(i + m) % n + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, t, Q;
    cin >> m >> n >> t >> Q;

    vector<double> p(m), q(n);
    for (auto &pi : p) cin >> pi;
    for (auto &qj : q) cin >> qj;

    vector<double> prob(m * n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) prob[i * n + j] = p[i] + q[j];

    SegmentTree st(m * n, prob);
    while (Q--) {
        int s;
        cin >> s;

        if (!s) {
            cout << "1\n";
            continue;
        }

        vector<vector<double>> dp(s);
        vector<int> indices(s);
        for (int k = 0; k < s; k++) {
            int i, j;
            cin >> i >> j;
            i--;
            j--;

            dp[k] = {1 - prob[i * n + j], prob[i * n + j]};
            indices[k] = i * n + j + 1;
        }

        for (int i = 1; i < s; i <<= 1)
            for (int j = 0; j < s - i; j += i << 1) dp[j] = convolve(dp[j], dp[i + j]);

        sort(indices.begin(), indices.end());
        auto seg = st.range_query(indices);
        for (int i = 0; i <= s; i++) {
            if (!(0 <= t - i - seg.offset && t - i - seg.offset < seg.poly.size())) {
                cout << "0 ";
                continue;
            }

            cout << fixed << setprecision(7) << seg.poly[t - i - seg.offset] * dp[0][i] / st[1].poly[t - st[1].offset] << " ";
        }
        cout << "\n";
    }
}
