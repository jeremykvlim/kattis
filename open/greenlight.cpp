#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tg, ty, tr, n;
    cin >> tg >> ty >> tr >> n;

    int cycle = tg + ty + tr;
    map<int, int> count;

    auto add = [&](int l, int r) {
        auto normalize = [&](int &t) {
            t = (t + cycle) % cycle;
        };
        normalize(l);
        normalize(r);

        count[l]++;
        count[r]--;
        if (l >= r) {
            count[0]++;
            count[cycle]--;
        }
    };

    auto process = [&]() {
        int t;
        string c;
        cin >> t >> c;

        if (c == "green") add(t - tg, t);
        else if (c == "yellow") add(t - tg - ty, t - tg);
        else add(t - tg - ty - tr, t - tg - ty);
    };

    auto time = [&](int target) {
        int time = 0, sum = 0, prev = 0;
        for (auto [curr, f] : count) {
            if (sum == target) time += curr - prev;
            sum += f;
            prev = curr;
        }
        return time;
    };

    for (int _ = 0; _ < n; _++) process();
    int t1 = time(n);

    if (!t1) {
        cout << 0;
        exit(0);
    }

    process();
    int t2 = time(n + 1);
    cout << fixed << setprecision(3) << (double) t2 / t1;
}
