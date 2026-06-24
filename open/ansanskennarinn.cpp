#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if constexpr (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    else return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    int g = 0;
    for (int i = 1; i < n; i++) g = gcd(g, abs(x[i] - x[i - 1]));
    cout << sgn(x[1] - x[0]) * g << "\n";

    sort(x.begin(), x.end());
    x.erase(unique(x.begin(), x.end()), x.end());
    for (int i = 0, xi = x.front(); xi <= x.back(); xi += g)
        if (i < x.size() && x[i] == xi) i++;
        else cout << xi << "\n";
}
