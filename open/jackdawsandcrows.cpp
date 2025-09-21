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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c, R;
    cin >> n >> c >> R;

    vector<int> s(n);
    for (int &si : s) cin >> si;

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return abs(s[i]) > abs(s[j]); });

    vector<int> nonzero;
    auto time = 0LL;
    for (int si : s)
        if (!si) time += R;
        else nonzero.emplace_back(si);

    for (int i = 0, j = 1; i < nonzero.size(); i = j++)
        while (j < nonzero.size() && sgn(nonzero[i]) == sgn(nonzero[j])) {
            time += R;
            j++;
        }

    auto check = [&](int l, int r) -> bool {
        if (l == -1 || r == n) return false;
        return ((r - l - 1) & 1) ^ (sgn(s[l]) == sgn(s[r]));
    };

    set<int> seen{-1, n};
    int reports = 0;
    for (int i : indices) {
        time = min(time, (long long) c * (abs(s[i]) + 1) + (long long) R * reports);
        if (!s[i]) break;

        auto it = seen.lower_bound(i);
        int l = *prev(it), r = *it;
        reports += check(l, i) + check(i, r) - check(l, r);
        seen.emplace(i);
    }
    cout << time;
}
