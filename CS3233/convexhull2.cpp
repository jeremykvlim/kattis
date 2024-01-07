#include <bits/stdc++.h>
using namespace std;

long long cross(pair<long long, long long> p1, pair<long long, long long> p2) {
    return p1.first * p2.second - p1.second * p2.first;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> points;
    while (n--) {
        long long x, y;
        char c;
        cin >> x >> y >> c;
        
        if (c == 'Y') points.emplace_back(x, y);
    }
    sort(points.begin(), points.end(), [&](auto p1, auto p2) {return p1.first != p2.first ? p1.first < p2.first : p1.second < p2.second;});

    vector<pair<long long, long long>> top, bot;
    for (auto &p : points) {
        while (top.size() > 1 && cross({top[top.size() - 1].first - top[top.size() - 2].first, top[top.size() - 1].second - top[top.size() - 2].second},
                                       {p.first - top.back().first, p.second - top.back().second}) > 0) top.pop_back();
        top.emplace_back(p);

        while (bot.size() > 1 && cross({bot[bot.size() - 1].first - bot[bot.size() - 2].first, bot[bot.size() - 1].second - bot[bot.size() - 2].second},
                                       {p.first - bot.back().first, p.second - bot.back().second}) < 0) bot.pop_back();
        bot.emplace_back(p);
    }

    cout << points.size() << "\n";
    for (int i = 0; i < bot.size() - 1; i++) cout << bot[i].first << " " << bot[i].second << "\n";
    for (int i = top.size() - 1; i; i--) cout << top[i].first << " " << top[i].second << "\n";
}
