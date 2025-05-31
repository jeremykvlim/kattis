#include <bits/stdc++.h>
using namespace std;

template <typename T>
T C(long long n, int k) {
    if (!k) return 1;
    return C<T>(n - 1, k - 1) * n / k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int m, a, b;
    cin >> n >> m >> a >> b;

    if (b == 1) {
        cout << n;
        exit(0);
    }

    auto locations = n * (b - 1);
    if (m > locations) {
        cout << n * b;
        exit(0);
    }

    auto choices = C<long double>(locations, m);
    auto prob = [&](int l, int cuts) {
        return C<long double>(locations - l, m - cuts) / choices;
    };

    auto p0 = prob(a - 1, 0);
    vector<long double> p1(b, 0), p2(a + 2, 0);
    for (int l = 1; l < b; l++) p1[l] = prob(l, 1);
    if (m >= 2)
        for (int l = 2; l < a + 2; l++) p2[l] = prob(l, 2);

    auto E = 0.L;
    for (int i = 1; i <= b; i++) {
        if (a == b) E += p0;
        if (a >= i)
            for (int l = i; l <= min(a, b - 1); l++) E += p1[l];

        for (int l1 = 1; l1 < i; l1++) {
            if (a >= b - l1) E += p1[b - l1];
            if (m == 1) continue;

            for (int l2 = i; l2 <= min(a + l1, b - 1); l2++) {
                int l = l2 - l1 + 1;
                if (a + 1 >= l) E += p2[l];
            }
        }
    }
    cout << fixed << setprecision(6) << n * E;
}
