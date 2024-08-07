#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    double p;
    cin >> n >> l >> p;

    priority_queue<double, vector<double>, greater<>> pq;
    for (int i = 0; i < n; i++) {
        double s;
        cin >> s;

        pq.emplace(1 / s);
    }

    double d = 1;
    while (l > 0) {
        auto prob = pq.top();
        pq.pop();

        int c = (l + n - 1) / n;
        pq.emplace(pow(prob, pow(p, c)));
        l -= c;
    }

    while (!pq.empty()) {
        d *= pq.top();
        pq.pop();
    }

    cout << d;
}
