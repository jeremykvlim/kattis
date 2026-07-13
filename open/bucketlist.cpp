#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, A0, B0;
    cin >> n >> A0 >> B0;

    int sum_alpha = 0, sum_beta = 0;
    vector<int> a(n), b(n), alpha(n), beta(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i] >> b[i] >> alpha[i] >> beta[i];

        a[i] += alpha[i] - A0;
        b[i] -= beta[i] + B0;
        sum_alpha += alpha[i];
        sum_beta += beta[i];
    }
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return a[i] > a[j]; });

    vector<int> p;
    priority_queue<pair<int, int>> pq;
    for (int _ = 0, k = 0; _ < n; _++) {
        while (k < n && a[order[k]] >= sum_alpha) {
            int i = order[k++];
            pq.emplace(-b[i], i);
        }

        if (pq.empty() || pq.top().first < sum_beta) {
            cout << "NO";
            exit(0);
        }

        int i = pq.top().second;
        pq.pop();

        p.emplace_back(i);
        sum_alpha -= alpha[i];
        sum_beta -= beta[i];
    }

    cout << "YES\n";
    reverse(p.begin(), p.end());
    for (int pi : p) cout << pi + 1 << " ";
}
