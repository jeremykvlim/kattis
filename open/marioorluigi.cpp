#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long b = 1e18, y = 0;
    vector<pair<long long, bool>> histogram{{0, false}, {1e18, true}};
    auto p = [&](long long x) {
        double u = 0, v = 0;
        for (int i = 0, pref_y = 0; i + 1 < histogram.size(); i++) {
            int lies = histogram.size() - (i + y - 2 * (pref_y += histogram[i].second));
            auto w = pow(1. / 13, lies) * pow(12. / 13, lies + histogram.size());

            u += ((double) (min(histogram[i + 1].first, x) - min(histogram[i].first, x)) / x) * w;
            v += ((double) (histogram[i + 1].first - histogram[i].first) / b) * w;
        }
        return (u / v) * ((double) x / b);
    };

    auto x = 0LL;
    for (int q = 0; q < 190; q++) {
        long long l = 0, r = LLONG_MAX, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (p(m) <= 0.5) l = m;
            else r = m;
        }
        x = l;

        auto a = x + (x && p(x) + p(x + 1) < 1);
        cout << "1 " << a << " " << b - 1 << "\n" << flush;

        string s;
        cin >> s;

        histogram.emplace_back(a, s == "YES");
        y += histogram.back().second;
        sort(histogram.begin(), histogram.end());
    }
    cout << "2 " << x << "\n" << flush;

    string s;
    cin >> s;
    cout << "! " << s << "\n" << flush;
}
