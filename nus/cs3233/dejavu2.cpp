#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, L;
    cin >> n >> k >> L;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;
    sort(h.rbegin(), h.rend());

    auto s = accumulate(h.begin(), h.begin() + k, 0LL);
    cout << s << "\n";

    priority_queue<tuple<long long, int, int, int>> pq;
    if (k < n) pq.emplace(s - h[k - 1] + h[k], k - 1, k, n);
    while (L-- > 1) {
        if (pq.empty()) {
            cout << "-1\n";
            continue;
        }

        auto [sum, i, l, r] = pq.top();
        pq.pop();

        cout << sum << "\n";
        if (i) pq.emplace(sum - h[i - 1] + h[i], i - 1, i, l);
        if (l + 1 < r) pq.emplace(sum - h[l] + h[l + 1], i, l + 1, r);
    }
}
