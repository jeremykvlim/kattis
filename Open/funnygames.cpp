#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        double x;
        int k;
        cin >> x >> k;

        vector<double> factor(k);
        for (auto &f : factor) cin >> f;
        sort(factor.begin(), factor.end());

        priority_queue<pair<double, double>, vector<pair<double, double>>, greater<>> pq;
        vector<pair<double, double>> ranges{{0.0, 1.0 / factor.front()}};
        while (ranges.back().second <= x) {
            auto &[l1, r1] = ranges.back();
            if (pq.empty())
                for (auto f : factor) pq.emplace(r1 / f, (r1 / factor.back()) / f);

            auto [l2, r2] = pq.top();
            pq.pop();

            if (l2 <= r1) r1 = max(r1, r2);
            else {
                for (auto f : factor) pq.emplace(r1 / f, min(l2, r1 / factor.back()) / f);

                auto [l3, r3] = pq.top();
                if (l2 > l3) {
                    pq.pop();
                    pq.emplace(l2, r2);
                    ranges.emplace_back(l3, r3);
                } else ranges.emplace_back(l2, r2);
            }
        }

        cout << (any_of(ranges.begin(), ranges.end(), [x](auto p) {return p.first < x && x <= p.second;}) ? "Nils\n" : "Mikael\n");
    }
}
