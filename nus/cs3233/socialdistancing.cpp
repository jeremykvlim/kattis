#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto dist = 0LL;
    priority_queue<long long> pq;
    for (int i = 1; i < n; i++) {
        pq.emplace(a[i]);
        dist += a[i];

        if (!pq.empty() && pq.top() > a[i]) {
            dist -= pq.top() - a[i];
            pq.pop();
            pq.emplace(a[i]);
        }
    }
    cout << dist;
}
