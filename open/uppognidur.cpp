#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n < 3) {
        cout << -1;
        exit(0);
    }

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    stack<pair<int, int>> s;
    for (int i = n - 1, j = -1, k = -1, xk = -1; ~i; i--) {
        if (~xk && xk > x[i]) {
            cout << i + 1 << " " << j + 1 << " " << k + 1;
            exit(0);
        }

        while (!s.empty() && x[i] > s.top().first) {
            auto [xl, l] = s.top();
            s.pop();
            
            if (xk < xl) {
                xk = xl;
                j = i;
                k = l;
            }
        }
        s.emplace(x[i], i);
    }
    cout << -1;
}
