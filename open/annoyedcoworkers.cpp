#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, c;
    cin >> h >> c;

    priority_queue<pair<long long, long long>, vector<pair<long long, long long>>, greater<>> pq;
    auto max_a = 0LL;
    while (c--) {
        int a, d;
        cin >> a >> d;

        max_a = max(max_a, (long long) a);
        pq.emplace((a + d), d);
    }

    while (h--) {
        auto [a, d] = pq.top();
        pq.pop();

        max_a = max(max_a, a);
        pq.emplace(a + d, d);
    }
    cout << max_a;
}
