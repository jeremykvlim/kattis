#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Fraction : array<T, 2> {
    using F = array<T, 2>;

    Fraction() = default;
    Fraction(T n, T d) : F{n, d} {
        reduce();
    }

    T & numer() {
        return (*this)[0];
    }

    T & denom() {
        return (*this)[1];
    }

    const T & numer() const {
        return (*this)[0];
    }

    const T & denom() const {
        return (*this)[1];
    }

    void reduce() {
        if (denom() < 0) {
            numer() *= -1;
            denom() *= -1;
        }

        T g = __gcd(abs(numer()), denom());
        if (g) {
            numer() /= g;
            denom() /= g;
        }
    }

    bool operator<(const Fraction &f) const {
        return numer() * f.denom() < f.numer() * denom();
    }

    bool operator>(const Fraction &f) const {
        return numer() * f.denom() > f.numer() * denom();
    }

    bool operator==(const Fraction &f) const {
        return numer() == f.numer() && denom() == f.denom();
    }

    bool operator!=(const Fraction &f) const {
        return numer() != f.numer() || denom() != f.denom();
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    Fraction operator+(const Fraction &f) const {
        return {numer() * f.denom() + f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator+(const T &v) const {
        return {numer() + v * denom(), denom()};
    }

    Fraction & operator+=(const Fraction &f) {
        numer() = numer() * f.denom() + f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer() += v * denom();
        reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer() * f.denom() - f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator-(const T &v) const {
        return {numer() - v * denom(), denom()};
    }

    Fraction & operator-=(const Fraction &f) {
        numer() = numer() * f.denom() - f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer() -= v * denom();
        reduce();
        return *this;
    }

    Fraction operator*(const Fraction &f) const {
        return {numer() * f.numer(), denom() * f.denom()};
    }

    Fraction operator*(const T &v) const {
        return {numer() * v, denom()};
    }

    Fraction & operator*=(const Fraction &f) {
        numer() *= f.numer();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator*=(const T &v) {
        numer() *= v;
        reduce();
        return *this;
    }

    Fraction operator/(const Fraction &f) const {
        return {numer() * f.denom(), denom() * f.numer()};
    }

    Fraction operator/(const T &v) const {
        return {numer(), denom() * v};
    }

    Fraction & operator/=(const Fraction &f) {
        numer() *= f.denom();
        denom() *= f.numer();
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom() *= v;
        reduce();
        return *this;
    }
};

struct SegmentTree {
    struct Segment {
        long long value, freq;

        Segment() : value(0), freq(0) {}

        auto & operator=(const long long &v) {
            freq = v;
            return *this;
        }

        auto operator+=(const long long &v) {
            value += v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            if (value < seg.value) return *this;
            else if (value > seg.value) return seg;
            else freq += seg.freq;

            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n, h;
    vector<Segment> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const long long &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy[j]) {
                apply(j << 1, lazy[j]);
                apply(j << 1 | 1, lazy[j]);
                lazy[j] = 0;
            }
        }
    }

    void assign(int i, const long long &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void modify(int l, int r, const long long &v) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++, v);
            }
            if (r & 1) {
                cr = true;
                apply(--r, v);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
        }
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<long long> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

template <typename T>
unsigned long long area_of_union_of_rectangles(vector<array<T, 4>> rectangles) {
    if (rectangles.empty()) return 0;

    int n = rectangles.size();
    vector<pair<T, int>> y(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        y[2 * i] = {yd, 2 * i};
        y[2 * i + 1] = {yu, 2 * i + 1};
    }
    sort(y.begin(), y.end());

    vector<T> coords(2 * n);
    for (int i = 0; i < 2 * n; i++) coords[i] = y[i].first;
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    vector<T> y_gaps(coords.size() - 1);
    for (int i = 0; i < coords.size() - 1; i++) y_gaps[i] = coords[i + 1] - coords[i];
    for (auto [yi, i] : y) rectangles[i / 2][2 + (i & 1)] = lower_bound(coords.begin(), coords.end(), yi) - coords.begin();

    vector<array<T, 4>> sweep(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        sweep[2 * i] = {xl, yd, yu, 2 * i};
        sweep[2 * i + 1] = {xr, yd, yu, 2 * i + 1};
    }
    sort(sweep.begin(), sweep.end());

    auto a = 0ULL;
    SegmentTree st(y_gaps.size(), y_gaps);
    auto y_range = y.back().first - y.front().first, prev = sweep[0][0];
    for (auto [x, yd, yu, i] : sweep) {
        if (prev != x) {
            auto [v, f] = st[1];

            if (!v) a += (y_range - f) * (x - prev);
            else a += y_range * (x - prev);

            prev = x;
        }
        st.modify(yd, yu, i & 1 ? -1 : 1);
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, L, w;
    cin >> n >> L >> w;

    vector<array<long long, 3>> players(2 * n);
    for (auto &[x, y, r] : players) cin >> x >> y >> r;

    auto union_area = [&](const vector<array<long long, 3>> &team) {
        auto extra = 0ULL;
        auto NE = 0LL, NW = 0LL, SE = 0LL, SW = 0LL;
        for (auto [x, y, r] : team) {
            NE = max(NE, r - ((L - x) + y));
            NW = max(NW, r - (x + y));
            SE = max(SE, r - ((L - x) + (w - y)));
            SW = max(SW, r - (x + (w - y)));
        }
        extra -= NE * NE + NW * NW + SE * SE + SW * SW;
        extra *= 2;

        vector<array<long long, 4>> rectangles(team.size());
        vector<pair<long long, long long>> N, S, E, W;
        for (int i = 0; i < team.size(); i++) {
            auto [x, y, r] = team[i];
            rectangles[i] = {x + y - r, x + y + r, x - y - r, x - y + r};
            if (r > y) N.emplace_back(x - (r - y), x + (r - y));
            if (r > w - y) S.emplace_back(x - (r - (w - y)), x + (r - (w - y)));
            if (r > L - x) E.emplace_back(y - (r - (L - x)), y + (r - (L - x)));
            if (r > x) W.emplace_back(y - (r - x), y + (r - x));
        }

        for (auto intervals : {N, S, E, W}) {
            if (intervals.empty()) continue;

            sort(intervals.begin(), intervals.end());
            intervals.erase(unique(intervals.begin(), intervals.end()), intervals.end());

            sort(intervals.begin(), intervals.end(), [&](auto i1, auto i2) { return i1.first != i2.first ? i1.first < i2.first : i1.second > i2.second; });
            intervals.erase(unique(intervals.begin(), intervals.end(), [&](auto i1, auto i2) { return i1.first == i2.first; }), intervals.end());

            auto max_r = LLONG_MIN;
            intervals.erase(remove_if(intervals.begin(), intervals.end(), [&](auto i) { return ((max_r = max(max_r, i.second)) != i.second); }), intervals.end());

            __int128 a = 0;
            if (!intervals.empty()) {
                auto [l, r] = intervals.back();
                a = (r - l) * (r - l);
            }

            auto prev_l = LLONG_MIN, prev_r = LLONG_MIN;
            for (auto [l, r] : intervals) {
                if (prev_l != LLONG_MIN) {
                    a += (prev_r - prev_l) * (prev_r - prev_l);
                    if (prev_r > l) a -= (prev_r - l) * (prev_r - l);
                }
                tie(prev_l, prev_r) = tie(l, r);
            }
            extra += a;
        }

        return 2ULL * area_of_union_of_rectangles(rectangles) - extra;
    };

    auto P = union_area({players.begin(), players.begin() + n}),
         Q = union_area({players.begin() + n, players.end()}),
         PQ = union_area(players);

    Fraction<__int128> f(P + Q - PQ, P);
    cout << (long long) f.numer() << "/" << (long long) f.denom();
}
