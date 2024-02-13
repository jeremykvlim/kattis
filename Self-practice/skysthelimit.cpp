#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double k;
    cin >> n >> k;

    vector<double> houses(n + 2, 0);
    for (int i = 1; i <= n; i++) {
        cin >> houses[i];
        
        houses[i] -= k * i * (n + 1 - i);
    }

    deque<int> dq{0};
    for (int i = 1; i <= n + 1; i++) {
        while (dq.size() > 1 && (houses[i] - houses[dq[1]]) * (dq[0] - dq[1]) >= (houses[dq[0]] - houses[dq[1]]) * (i - dq[1])) 
            dq.pop_front();
        
        dq.emplace_front(i);
    }

    for (int i = dq.size() - 1; i; i--) {
        int l = dq[i], r = dq[i - 1];

        for (int j = l; j < r; j++) houses[j] = (houses[l] * (r - j) + houses[r] * (j - l)) / (r - l);
    }
    
    double height = 0;
    for (int i = 1; i <= n; i++) {
        houses[i] += k * i * (n + 1 - i);
        height = max(height, houses[i]);
    }

    cout << fixed << setprecision(6) << height;
}
