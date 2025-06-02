#include <bits/stdc++.h>
using namespace std;

template <typename T>
T heron(T s, T a, T b, T c) {
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    double total = 0;
    vector<double> segments(n);
    for (auto &len : segments) {
        cin >> len;

        total += len;
    }
    sort(segments.begin(), segments.end());

    while (!segments.empty() && 2 * segments.back() >= total) {
        total -= segments.back();
        segments.pop_back();
        n--;
    }

    if (n < 3) {
        cout << "0\n";
        exit(0);
    }

    auto area = [&]() {
        auto angle = [&](double m, int l, int r) {
            double a = 0;
            for (int i = l; i < r; i++) a += acos(1 - (segments[i] * segments[i] / (2 * m * m)));
            return a;
        };

        bool covered = angle(segments.back() / 2, 0, n) >= 2 * M_PI;
        double area = 0, l = segments.back() / 2, r = covered ? total / 4 : 1e6, m;
        while (l + 1e-10 < r && l + l * 1e-10 < r) {
            m = l + (r - l) / 2;

            if (covered ? (angle(m, 0, n) >= 2 * M_PI) : (angle(m, n - 1, n) >= angle(m, 0, n - 1))) l = m;
            else r = m;
        }

        for (auto len : segments) {
            auto s = (len + 2 * l) / 2;
            area += heron(s, r, r, len);
        }

        if (!covered) {
            auto len = segments.back(), s = (len + 2 * l) / 2;
            area -= 2 * heron(s, r, r, len);
            }

        return area;
    };
    auto a = area();

    if (n <= 4) {
        cout << fixed << setprecision(3) << a << "\n";
        exit(0);
    }

    total -= segments.back();
    segments.pop_back();
    n--;

    cout << fixed << setprecision(3) << max(a, area()) << "\n";
}
