#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AffineFunction {
    T m, c;
    int i;
    mutable T cutoff;

    AffineFunction(T m = 0, T c = numeric_limits<T>::lowest() / 4, int i = -1) : m(m), c(c), i(i), cutoff(0) {}

    T operator()(T x) const {
        return m * x + c;
    }

    bool operator<(const AffineFunction &f) const {
        return m < f.m;
    }

    bool operator<(T x) const {
        return cutoff < x;
    }
};

template <typename T>
struct DynamicHull : multiset<AffineFunction<T>, less<>> {
    T div(T a, T b) {
        if constexpr (!is_floating_point_v<T>) return a / b - ((a ^ b) < 0 && a % b);
        else return a / b;
    }

    bool update(auto it_l, auto it_r) {
        if (it_r == this->end()) {
            it_l->cutoff = numeric_limits<T>::max();
            return false;
        }

        if (it_l->m == it_r->m) it_l->cutoff = it_l->c > it_r->c ? numeric_limits<T>::max() : numeric_limits<T>::lowest();
        else it_l->cutoff = div(it_r->c - it_l->c, it_l->m - it_r->m);
        return it_l->cutoff >= it_r->cutoff;
    }

    void add(const AffineFunction<T> &f) {
        auto it = this->insert(f);
        for (auto it_r = next(it); update(it, it_r); it_r = this->erase(it_r));

        if (it != this->begin()) {
            auto it_l = prev(it);
            if (update(it_l, it)) {
                this->erase(it);
                it = it_l;
                update(it, next(it));
            } else it = it_l;
        }

        while (it != this->begin()) {
            auto it_l = prev(it);
            if (it_l->cutoff < it->cutoff) break;
            update(it_l, this->erase(it));
            it = it_l;
        }
    }

    void add(T m, T c, int i = -1) {
        add(AffineFunction<T>(m, c, i));
    }

    pair<T, int> query(T x) {
        auto f = *this->lower_bound(x);
        return {f(x), f.i};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int k = 0;
    vector<int> w(n), h(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> h[i];

        if (w[k] < w[i]) k = i;
    }
    rotate(w.begin(), w.begin() + k, w.end());
    rotate(h.begin(), h.begin() + k, h.end());

    auto sum = 0LL;
    vector<long long> dp(n + 1, 0);
    for (int _ = 0; _ < 2; _++) {
        DynamicHull<long long> dh1, dh2;
        for (int i = 0; i < n; i++) {
            dh1.add(w[i], dp[i], i);
            dh2.add(h[i], dp[i], i);
            dp[i + 1] = max(dh1.query(h[i]).first, dh2.query(w[i]).first);
        }
        sum = max(sum, dp[n]);
        fill(dp.begin(), dp.end(), 0);
        reverse(w.begin() + 1, w.end());
        reverse(h.begin() + 1, h.end());
    }
    cout << sum;
}
