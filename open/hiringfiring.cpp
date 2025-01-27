#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> fired(n);
    stack<pair<int, int>> employees;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < n; i++) {
        int f, h;
        cin >> f >> h;

        while (f) {
            if (!employees.top().first) {
                employees.pop();
                continue;
            }

            int layoffs = min(f, employees.top().first);
            employees.top().first -= layoffs;
            f -= layoffs;
            fired[i].emplace_back(employees.top().second);
            fired[employees.top().second].emplace_back(i);
        }

        employees.emplace(h, i);
        pq.emplace(0, i);
    }

    vector<int> awkward(n, 0), count(n, 0), hr(n, 0);
    while (!pq.empty()) {
        int day = pq.top().second;
        pq.pop();
        if (hr[day]) continue;

        hr[day] = 1;
        while ((awkward[day] >> hr[day]) & 1) hr[day]++;
        for (int d : fired[day])
            if (!hr[d] && !((awkward[day] >> hr[day]) & 1)) {
                awkward[d] |= 1 << hr[day];
                count[d]++;
                pq.emplace(-count[d], d);
            }
    }

    cout << *max_element(hr.begin(), hr.end()) << "\n";
    for (int id : hr) cout << id << " ";
}
