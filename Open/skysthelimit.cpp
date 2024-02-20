#include <bits/stdc++.h>
using namespace std;

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

    deque<int> convex{0};
    for (int i = 1; i <= n + 1; i++) {
        while (convex.size() > 1 && (houses[i] - houses[convex[1]]) * (convex[0] - convex[1]) >= (houses[convex[0]] - houses[convex[1]]) * (i - convex[1]))
            convex.pop_front();

        convex.emplace_front(i);
    }

    for (int i = convex.size() - 1; i; i--) {
        int l = convex[i], r = convex[i - 1];

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }

    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
