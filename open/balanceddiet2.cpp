#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k;
    cin >> m >> k;

    int sum = 0;
    vector<int> a(m);
    for (int &ai : a) {
        cin >> ai;

        sum += ai;
    }

    vector<int> count(m, 0);
    for (int i = 0; i < k; i++) {
        int b;
        cin >> b;

        count[b - 1]++;
    }

    auto f = [&](int i) -> int {
        return ((count[i] + 1LL) * sum - 1) / a[i];
    };

    priority_queue<pair<int, int>> pq;
    for (int i = 0; i < m; i++) pq.emplace(-f(i), i);
    for (int s = 0; s <= m; s++) {
        auto [fi, i] = pq.top();
        pq.pop();

        if (s && f(i) < s + k) {
            cout << s - 1;
            exit(0);
        }

        count[i]++;
        pq.emplace(-f(i), i);
    }
  
    cout << "forever";
}
