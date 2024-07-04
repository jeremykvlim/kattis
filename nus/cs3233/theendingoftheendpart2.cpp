#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;

    vector<pair<int, char>> routes;
    for (int i = 0; i < r + c - 2; i++) {
        char d;
        cin >> d;

        if (!routes.empty() && routes.back().second == d) routes.back().first++;
        else routes.emplace_back(1, d);
    }

    vector<long long> l(r, 1e11), d(c, 1e11);
    int n = 0, e = 0;
    long long li = 5e10, di = 5e10;
    for (int i = 0, j = routes.size() / 2; i < routes.size(); i++, j--) {
        auto estimate = [&](int &dir1, int &dir2, vector<long long> &strengths, long long &v) {
            dir1 += routes[i].first;
            strengths[dir2] = v;
            if (i < routes.size() - 1) v -= routes[i + 1].first * j;
        };
        routes[i].second == 'N' ? estimate(n, e, d, di) : estimate(e, n, l, li);
    }

    for (auto li : l) cout << li << " ";
    cout << "\n";
    for (auto di : d) cout << di << " ";
}
