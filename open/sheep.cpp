#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int total = 0;
    vector<int> segments(n);
    for (int &len : segments) {
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
        auto angle = [&](double m, int j, int k) -> double {
            double a = 0;
            for (int i = j; i < k; i++) a += acos(1 - (segments[i] * segments[i] / (2 * m * m)));
            return a;
        };
        
        bool covered = angle((double) segments.back() / 2, 0, n) >= 2 * M_PI;
        double area = 0, l = (double) segments.back() / 2, r = covered ? (double) total / 4 : 1e6, m;
        while (l + 1e-9 < r) {
            m = l + (r - l) / 2;

            if (covered ? (angle(m, 0, n) >= 2 * M_PI) : (angle(m, n - 1, n) >= angle(m, 0, n - 1))) l = m;
            else r = m;
        }

        for (int len : segments) {
            double s = (len + 2 * l) / 2;
            area += sqrt(s * abs(s - r) * abs(s - r) * abs(s - len));
        }

        if (!covered) {
            double s = (segments.back() + 2 * l) / 2;
            area -= 2 * sqrt(s * abs(s - r) * abs(s - r) * abs(s - segments.back()));
        }

        return area;
    };
    double a = area();

    if (n <= 4) {
        cout << fixed << setprecision(3) << a << "\n";
        exit(0);
    }
    
    total -= segments.back();
    segments.pop_back();
    n--;

    cout << fixed << setprecision(3) << max(a, area()) << "\n";
}
