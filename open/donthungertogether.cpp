#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, double>> food(n);
    for (auto &[f, q] : food) cin >> q >> f;

    double l = 0, r = INT_MAX, m;
    while (l + 1e-9 < r && l + l * 1e-9 < r) {
        m = l + (r - l) / 2;

        auto survive = [&]() {
            priority_queue<pair<int, double>, vector<pair<int, double>>, greater<>> pq;
            for (int i = 0; i < n; i++) {
                pq.emplace(food[i]);
                auto need = m;
                while (!pq.empty()) {
                    auto [f, q] = pq.top();
                    pq.pop();

                    if (f < i + 1) continue;

                    auto use = min(need, q);
                    need -= use;
                    q -= use;

                    if (q > 0) pq.emplace(f, q);
                    if (need <= 0) break;
                }

                if (need > 0) return false;
            }

            return true;
        };

        if (!survive()) r = m;
        else l = m;
    }

    cout << fixed << setprecision(9) << (l == 0 ? -1 : l / k);
}
