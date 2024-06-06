#include <bits/stdc++.h>
using namespace std;

double cross(pair<long long, long long> i, pair<long long, long long> j, pair<long long, long long> k) {
    return (double) (k.first - i.first) * (double) (j.second - i.second) - (double) (k.second - i.second) * (double) (j.first - i.first);
}

void read(deque<pair<int, long long>> &convex, int size) {
    for (int i = 0; i < size; i++) {
        long long s;
        cin >> s;

        while (convex.size() > 1 && cross(convex[1], convex[0], {i, s}) > 0)
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
    sort(route.begin(), route.end(), [&](auto t1, auto t2) {return get<0>(t1) * get<1>(t2) < get<1>(t1) * get<0>(t2);});

    for (auto [cost, count, direction] : route)
        while (count--) cout << direction;
}
