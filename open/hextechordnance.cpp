#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    if (n < 3) {
        cout << "unstable";
        exit(0);
    }

    vector<int> r(n - 1), min_a(n - 1), max_a(n - 1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (int i = 0; i < n - 1; i++) {
        r[i] = i + 1;
        min_a[i] = min(a[i], a[i + 1]);
        max_a[i] = max(a[i], a[i + 1]);
        pq.emplace(max_a[i] - min_a[i], i);
    }

    for (int k = 1; k <= n; k++) {
        if (pq.empty() || pq.top().first > k) {
            cout << "unstable";
            exit(0);
        }

        auto [diff, i] = pq.top();
        pq.pop();

        if (r[i] + 1 < n) {
            r[i]++;
            min_a[i] = min(min_a[i], a[r[i]]);
            max_a[i] = max(max_a[i], a[r[i]]);
            pq.emplace(max_a[i] - min_a[i], i);
        }
    }
    cout << "stable";
}
