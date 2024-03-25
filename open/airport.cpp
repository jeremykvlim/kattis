#include <bits/stdc++.h>
using namespace std;

double dot(pair<long long, long long> i, pair<long long, long long> j) {
    return (double) (i.first * j.first) + (double) (i.second * j.second);
}

double cross(pair<long long, long long> i, pair<long long, long long> j) {
    return (double) (i.first * j.second) - (double) (i.second * j.first);
}

double cross(pair<long long, long long> i, pair<long long, long long> j, pair<long long, long long> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    double len = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            vector<pair<double, int>> valid;
            auto p1 = points[i], p2 = points[j];
            auto diff1 = make_pair(p2.first - p1.first, p2.second - p1.second);
            for (int k = 0; k < n; k++) {
                auto p3 = points[k], p4 = points[(k + 1) % n], p5 = points[(k + n - 1) % n];
                auto diff2 = make_pair(p3.first - p1.first, p3.second - p1.second), diff3 = make_pair(p4.first - p3.first, p4.second - p3.second);

                auto c1 = cross(p1, p2, p3), c2 = cross(p1, p2, p4), c3 = cross(p1, p2, p5);
                auto s1 = (c1 > 0) - (c1 < 0), s2 = (c2 > 0) - (c2 < 0), s3 = (c3 > 0) - (c3 < 0);
                if (!s1 && !s2 && !s3) continue;

                if (!s1) {
                    double l = diff1.first ? (double) diff2.first / diff1.first : (double) diff2.second / diff1.second;

                    if (s2 && !s3) valid.emplace_back(l, s2);
                    else if (s3 && !s2) valid.emplace_back(l, s3);
                    else if (s2 != s3) valid.emplace_back(l, 0);
                } else if (s1 == -s2) valid.emplace_back(cross(diff2, diff3) / cross(diff1, diff3), 0);
            }
            sort(valid.begin(), valid.end());

            bool inside = false;
            int sign = 0;
            double prev = 0, sum = 0, l = 0;
            for (auto [curr, s] : valid) {
                if (sign || inside) {
                    sum += curr - prev;
                    l = max(l, sum);
                }

                if (sign) {
                    if (sign != s) inside = !inside;
                    sign = 0;
                } else if (!s) {
                    inside = !inside;
                    sum = 0;
                } else sign = s;

                if (!inside) sum = 0;
                prev = curr;
            }

            len = max(len, l * sqrt(dot(diff1, diff1)));
        }

    cout << fixed << setprecision(6) << len;
}
