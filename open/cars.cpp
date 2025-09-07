#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    
    auto read = [&]() -> array<double, 6> {
        char d;
        double x, y, s, w, l;
        cin >> d >> x >> y >> s >> w >> l;
        w *= 0.5;
        l *= 0.5;

        unordered_map<char, array<int, 2>> directions{{'N', {0, 1}}, {'S', {0, -1}}, {'W', {-1, 0}}, {'E', {1, 0}}};
        auto dx = directions[d][0] * s, dy = directions[d][1] * s;
        if (dx) swap(w, l);
        return {x, y, dx, dy, w, l};
    };

    auto car1 = read(), car2 = read();
    
    auto interval = [&](double pos, double speed, double dimension) -> pair<double, double> {
        if (!speed) return abs(pos) < dimension ? make_pair(INT_MIN, INT_MAX) : make_pair(0, 0);
        auto t1 = - (dimension + pos) / speed, t2 = (dimension - pos) / speed;

        return minmax(t1, t2);
    };
    
    vector<pair<double, double>> times{{0, t},
                                       interval(car1[0] - car2[0], car1[2] - car2[2], car1[4] + car2[4]),
                                       interval(car1[1] - car2[1], car1[3] - car2[3], car1[5] + car2[5])};
    cout << (max({times[0].first, times[1].first, times[2].first}) < min({times[0].second, times[1].second, times[2].second}) ? "crash" : "safe");
}
