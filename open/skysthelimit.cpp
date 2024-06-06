#include <bits/stdc++.h>
using namespace std;

double cross(pair<int, double> i, pair<int, double> j, pair<int, double> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double k;
    cin >> n >> k;

    vector<double> houses(n + 2, 0);
    for (int i = 1; i <= n; i++) {
        cin >> houses[i];

        houses[i] -= k * i * (n + 1 - i);
    }

    deque<pair<int, double>> convex{{0, houses[0]}};
    for (int i = 1; i <= n + 1; i++) {
        while (convex.size() > 1 && cross(convex[1], {i, houses[i]}, convex[0]) > 0)
            convex.pop_front();

        convex.emplace_front(i, houses[i]);
    }

    for (int i = convex.size() - 1; i; i--) {
        int l = convex[i].first, r = convex[i - 1].first;

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
