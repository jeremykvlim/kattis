#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, g;
    cin >> n >> g;

    auto d_max = 0LL;
    vector<pair<long long, int>> stations(n);
    for (auto &[d, c] : stations) {
        cin >> d >> c;

        d_max = max(d_max, d);
    }
    stations.emplace_back(0, 1e9);
    stations.emplace_back(d_max, 1e9);
    sort(stations.begin(), stations.end());

    for (int i = 1; i < n + 2; i++)
        if (stations[i].first - stations[i - 1].first > g) {
            cout << "cancel road trip";
            exit(0);
        }

    stack<int> mono_inc;
    vector<int> next(n + 2, n + 1);
    for (int i = 0; i < n + 2; i++) {
        while (!mono_inc.empty() && stations[i].second < stations[mono_inc.top()].second) {
            next[mono_inc.top()] = i;
            mono_inc.pop();
        }
        mono_inc.emplace(i);
    }

    auto C = 0LL;
    for (int i = 0, gas = g; i < n + 1; i++) {
        auto need = min(stations[next[i]].first - stations[i].first, (long long) g);
        if (need > gas) {
            auto refuel = need - gas;
            C += refuel * stations[i].second;
            gas += refuel;
        }
        gas -= stations[i + 1].first - stations[i].first;
    }
    cout << C;
}
