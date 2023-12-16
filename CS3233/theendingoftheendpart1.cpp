#include <bits/stdc++.h>
using namespace std;

void read(vector<pair<int, long long>> &d, int size) {
    for (int i = 0; i < size; i++) {
        long long s;
        cin >> s;
        
        while (d.size() > 1 &&
               (d.back().first - d[d.size() - 2].first) * (d.back().second - s) > (d.back().first - i) * (d.back().second - d[d.size() - 2].second)) d.pop_back();
        d.emplace_back(i, s);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<pair<int, long long>> east, north;
    read(east, r);
    read(north, c);

    vector<tuple<long long, int, char>> route;
    for (int i = 1; i < east.size(); i++) route.emplace_back(east[i].second - east[i - 1].second, east[i].first - east[i - 1].first, 'N');
    for (int i = 1; i < north.size(); i++) route.emplace_back(north[i].second - north[i - 1].second, north[i].first - north[i - 1].first, 'E');
    sort(route.begin(), route.end(), [&](auto move1, auto move2) {return get<0>(move1) * get<1>(move2) < get<1>(move1) * get<0>(move2);});

    for (auto [cost, count, direction] : route)
        while (count--) cout << direction;
}
