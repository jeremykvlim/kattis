#include <bits/stdc++.h>
using namespace std;

void read(deque<pair<int, long long>> &convex, int size) {
    for (int i = 0; i < size; i++) {
        long long s;
        cin >> s;

        while (convex.size() > 1 && (convex[0].first - convex[1].first) * (convex[0].second - s) > (convex[0].first - i) * (convex[0].second - convex[1].second))
            convex.pop_front();

        convex.emplace_front(i, s);
    }

    reverse(convex.begin(), convex.end());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    deque<pair<int, long long>> east, north;
    read(east, r);
    read(north, c);

    vector<tuple<long long, int, char>> route;
    for (int i = 1; i < east.size(); i++) route.emplace_back(east[i].second - east[i - 1].second, east[i].first - east[i - 1].first, 'N');
    for (int i = 1; i < north.size(); i++) route.emplace_back(north[i].second - north[i - 1].second, north[i].first - north[i - 1].first, 'E');
    sort(route.begin(), route.end(), [&](auto move1, auto move2) {return get<0>(move1) * get<1>(move2) < get<1>(move1) * get<0>(move2);});

    for (auto [cost, count, direction] : route)
        while (count--) cout << direction;
}
