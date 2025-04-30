#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s, a;
    cin >> n >> s >> a;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;

    auto valid = [&](auto m) {
        auto sum = 0LL;
        for (int hi : h) {
            auto life = -(m * a - hi);
            if (life > 0) {
                sum += (life + s - 1) / s;
                if (sum > m) return false;
            }
        }
        return sum <= m;
    };

    long long l = 0, r = 0, m;
    for (int hi : h) r += (hi + s - 1) / s;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (valid(m)) r = m;
        else l = m;
    }
    cout << r;
}
