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

    int r;
    cin >> r;

    int n = 1 << r;
    vector<int> s(n);
    for (int &si : s) cin >> si;

    vector<long long> dp(n), temp(n);
    auto dnc = [&](auto &&self, int l, int r) -> vector<int> {
        if (l + 1 == r) return {l};

        int m = l + (r - l) / 2;
        auto left = self(self, l, m), right = self(self, m, r);

        auto update = [&](const auto &v1, const auto &v2) {
            vector<long long> pref(v1.size() + 1, 1e18);
            for (int i = 0; i < v1.size(); i++) pref[i + 1] = min(pref[i], dp[v1[i]]);

            DynamicHull<long long> dh;
            dh.add(0, -1e18);
            for (int i = v1.size() - 1, j = v2.size() - 1; ~j; j--) {
                for (; ~i && s[v1[i]] > s[v2[j]]; i--) dh.add(2 * s[v1[i]], -dp[v1[i]] - (long long) s[v1[i]] * s[v1[i]]);
                temp[v2[j]] = dp[v2[j]] + min(pref[i + 1], (long long) s[v2[j]] * s[v2[j]] - dh.query(s[v2[j]]).first);
            }
        };
        update(right, left);
        update(left, right);

        vector<int> both;
        merge(left.begin(), left.end(), right.begin(), right.end(), back_inserter(both), [&](int i, int j) { return s[i] != s[j] ? s[i] < s[j] : i < j; });
        for (int i : both) dp[i] = temp[i];
        return both;
    };
    dnc(dnc, 0, n);
    cout << dp[0];
}
