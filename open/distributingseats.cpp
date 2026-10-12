#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, c;
    cin >> n >> r >> c;

    vector<pair<int, int>> intervals(n);
    for (auto &[L, R] : intervals) {
        int a, b, s;
        cin >> a >> b >> s;

        L = max(a - s, 1);
        R = min(a + s, r);
    }
    sort(intervals.begin(), intervals.end());

    priority_queue<int, vector<int>, greater<>> pq;
    int count = 0;
    for (int i = 1, j = 0; i <= r; i++) {
        while (!pq.empty() && pq.top() < i) pq.pop();

        for (; j < n && intervals[j].first == i; j++) pq.emplace(intervals[j].second);

        for (int _ = 0; _ < c && !pq.empty(); _++) {
            pq.pop();
            count++;
        }
    }
    cout << count;
}
