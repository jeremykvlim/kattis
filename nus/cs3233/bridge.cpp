#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> times(n);
    for (int &t : times) cin >> t;
    sort(times.begin(), times.end());

    int total = 0;
    vector<vector<int>> moves;
    auto add1 = [&](int x) {
        moves.emplace_back();
        moves.back().emplace_back(x);
    };
    auto add2 = [&](int x, int y) {
        moves.emplace_back();
        moves.back().emplace_back(x);
        moves.back().emplace_back(y);
    };

    for (; n > 3; n -= 2) {
        int t1 = times[0], t2 = times[1], t3 = times[n - 2], t4 = times[n - 1],
            tl = t1 + 2 * t2 + t4, tr = t1 * 2 + t3 + t4;

        if (tl <= tr) {
            total += tl;
            add2(t1, t2);
            add1(t1);
            add2(t3, t4);
            add1(t2);
        } else {
            total += tr;
            add2(t1, t4);
            add1(t1);
            add2(t1, t3);
            add1(t1);
        }
    }

    if (n == 1) {
        total += times[0];
        add1(times[0]);
    } else if (n == 2) {
        total += times[1];
        add2(times[0], times[1]);
    } else if (n == 3) {
        total += times[0] + times[1] + times[2];
        add2(times[0], times[1]);
        add1(times[0]);
        add2(times[0], times[2]);
    }

    cout << total << "\n";
    for (auto &m : moves) {
        for (int t : m) cout << t << " ";
        cout << "\n";
    }
}
