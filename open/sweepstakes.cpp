#include <bits/stdc++.h>
using namespace std;

vector<double> convolve(const vector<double> &a, const vector<double> &b) {
    int n = 1;
    while (n < a.size() + b.size() - 1) n <<= 1;

    vector<int> rev(n, 0);
    for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << __lg(n)) >> 1;

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

        Segment() : poly{1}, offset(0) {}

        auto & operator=(double v) {
            if (v == 0) poly = {1};
            else poly = {1 - v, v};

            return *this;
        }

        auto operator+=(Segment seg) {
            auto c = convolve(poly, seg.poly);

            int l = find_if(c.begin(), c.end(), [](auto value) {return value > 1e-11;}) - c.begin(),
                    r = find_if(c.rbegin(), c.rend(), [](auto value) {return value > 1e-11;}) - c.rbegin();

            poly = {c.begin() + l, c.end() - r};
            offset += seg.offset + l;
            return *this;
        }

        friend auto operator+(Segment sl, Segment sr) {
            if (sl.poly.size() == 1 && sl.poly[0] == 1) return sr;
            if (sr.poly.size() == 1 && sr.poly[0] == 1) return sl;
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

    Segment query(int v, vector<int> &indices, int li, int ri, int l, int r) {
        if (li == ri) return ST[v];
        if (l == r) return {};

        int m = midpoint(l, r), mi = upper_bound(indices.begin() + li, indices.begin() + ri, m) - indices.begin();
        return query(v << 1, indices, li, mi, l, m) + query(v << 1 | 1, indices, mi, ri, m + 1, r);
    }

    Segment query(vector<int> &indices, int li, int ri) {
        return query(1, indices, li, ri, 1, n);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, vector<double> &a) : n(n), ST(2 * n) {
        int bit_ceil = 1;
        while (bit_ceil < n) bit_ceil <<= 1;
        for (int i = 0; i < a.size(); i++) ST[(i + bit_ceil) % n + n] = a[i];
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
        auto seg = st.query(indices, 0, s);
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
