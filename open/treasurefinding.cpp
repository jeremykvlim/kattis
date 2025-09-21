#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
array<T, 3> horners_method(const vector<T> &a, T x) {
    T f = a.back(), df = 0, ddf = 0;
    for (int i = a.size() - 2; ~i; i--) {
        ddf = ddf * x + 2 * df;
        df = df * x + f;
        f = f * x + a[i];
    }
    return {f, df, ddf};
}

template <typename T>
T polyroot(const vector<T> &a, T xl = 0, T xr = 0) {
    if (!sgn(a[0])) return (T) 0;

    auto radius_of_convergence = [&]() {
        int n = a.size() - 1;
        for (; n && !sgn(a[n]); n--);

        T R = 0;
        for (T ai : a) R = max(R, fabs(ai / a[n]));
        return (T) (R + 1);
    };

    if (!sgn(xl) && !sgn(xr)) {
        T R = radius_of_convergence();
        xl = -R;
        xr = R;
    }

    auto seed = [&]() {
        auto valid = [&](T x) -> bool {
            auto [f, df, ddf] = horners_method(a, x);
            return sgn(df) && sgn(f * ddf) == 1;
        };
        if (valid(xl)) return xl;
        if (valid(xr)) return xr;

        int lg = ceil(log2(xr - xl + 1));
        for (int i = 0; i < lg; i++) {
            T x = xl + (xr - xl) * i / lg;
            if (valid(x)) return x;
        }
        return (xl + xr) / 2;
    };

    T x0 = seed();
    for (;;) {
        auto [f, df, ddf] = horners_method(a, x0);
        if (!sgn(df)) return x0;

        T step = f / df, x1 = x0 - step;
        if (sgn(fabs(step) / (fabs(x1) + 1)) <= 0) return x1;

        if (x1 < xl) x1 = xl;
        else if (x1 > xr) x1 = xr;
        x0 = x1;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        int p;
        cin >> p;

        vector<double> a(p + 1);
        for (auto &ai : a) cin >> ai;
        cout << round(polyroot(a, -1e4, 1e4)) << "\n";
    }
}
