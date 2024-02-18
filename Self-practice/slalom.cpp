#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        double x, y;
        cin >> x >> y;

        vector<pair<double, double>> points;
        vector<double> X;
        points.emplace_back(x, y);
        X.emplace_back(x);

        for (int i = 0; i < n - 1; i++) {
            double x1, x2;
            cin >> y >> x1 >> x2;

            points.emplace_back(x1, y);
            points.emplace_back(x2, y);
            X.emplace_back(x1);
            X.emplace_back(x2);
        }

        double x1, x2;
        cin >> y >> x1 >> x2;

        X.emplace_back(x1);
        X.emplace_back(x2);
        sort(X.begin(), X.end());
        X.erase(unique(X.begin(), X.end()), X.end());

        for (auto x : X)
            if (x1 <= x && x <= x2) points.emplace_back(x, y);

        vector<vector<pair<int, double>>> adj_list(points.size());
        for (int i = 0; i <= 2 * (n - 1); i++) {
            double l = INT_MIN, r = INT_MAX;
            auto [x1, y1] = points[i];

            for (int j = (i & 1) ? i + 2 : i + 1; j <= 2 * (n - 1) && l <= r; j++) {
                auto [x2, y2] = points[j];
                auto gradient = (x2 - x1) / (y2 - y1);

                if (l <= gradient && gradient <= r) adj_list[i].emplace_back(j, hypot(x2 - x1, y2 - y1));

                if (j & 1) r = min(r, gradient);
                else l = max(l, gradient);
            }

            for (int j = 2 * (n - 1) + 1; j < points.size(); j++) {
                auto [x2, y2] = points[j];
                auto gradient = (x2 - x1) / (y2 - y1);

                if (l <= gradient && gradient <= r) adj_list[i].emplace_back(j, hypot(x2 - x1, y2 - y1));
            }
        }

        vector<int> degree(points.size(), 0);
        for (int v = 0; v < points.size(); v++)
            for (auto [u, w] : adj_list[v]) degree[u]++;

        vector<double> dist(points.size(), INT_MAX);
        dist[0] = 0;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, w] : adj_list[v]) {
                dist[u] = min(dist[u], dist[v] + w);
                if (!--degree[u]) q.emplace(u);
            }
        }

        cout << fixed << setprecision(7) << *min_element(dist.begin() + 2 * (n - 1) + 1, dist.end()) << "\n";
    }
}
