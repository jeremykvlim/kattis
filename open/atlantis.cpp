#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> stores(n);
    for (auto &[h, t] : stores) cin >> t >> h;
    sort(stores.begin(), stores.end());

    auto sum = 0LL;
    priority_queue<long long> pq;
    for (auto [h, t] : stores) {
        sum += t;
        pq.emplace(t);
        if (sum > h) {
            sum -= pq.top();
            pq.pop();
        }
    }
    cout << pq.size();
}
