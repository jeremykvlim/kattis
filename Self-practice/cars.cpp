#include <bits/stdc++.h>
using namespace std;

pair<double, double> interval(double position, double speed, double dimension) {
    if (!speed) return abs(position) < dimension ? make_pair(INT_MIN, INT_MAX) : make_pair(0, 0);
    double t1 = (-dimension - position) / speed, t2 = (dimension - position) / speed;

    return {min(t1, t2), max(t1, t2)};
}

bool crash(double t, vector<double> car1, vector<double> car2) {
    vector<pair<double, double>> times = {{0, t},
                                          interval(car1[0] - car2[0], car1[2] - car2[2], car1[4] + car2[4]),
                                          interval(car1[1] - car2[1], car1[3] - car2[3], car1[5] + car2[5])};

    return max(times[0].first, max(times[1].first, times[2].first)) < min(times[0].second, min(times[1].second, times[2].second));
}

vector<double> car() {
    char d;
    double x, y, s, w, l;
    cin >> d >> x >> y >> s >> w >> l;
    w /= 2.0, l /= 2.0;
    
    unordered_map<char, pair<int, int>> directions = {{'N', {0, 1}}, {'S', {0, -1}}, {'W', {-1, 0}}, {'E', {1, 0}}};
    auto dir = directions[d];
    auto dx = dir.first * s, dy = dir.second * s;
    if (dx) swap(w, l);
    return vector<double>{x, y, dx, dy, w, l};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<double> car1 = car(), car2 = car();
    cout << (crash(t, car1, car2) ? "crash" : "safe");
}
