#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, n;
    cin >> w >> n;

    vector<int> dp1(w + 1, 1e9), dp2(w + 1, 1e9);
    dp1[0] = dp2[0] = 0;
    for (int _ = 0; _ < n; _++) {
        int a;
        cin >> a;

        vector<int> temp1(w + 1, 1e9), temp2(w + 1, 1e9);
        for (int i = 0; i <= w; i++) {
            if (dp1[i] != 1e9) {
                int c = dp1[i];
                auto update = [&](int i, int j) {
                    int k = i + a, cost = c + !i;
                    if (k > w) {
                        k = a;
                        cost++;
                    }
                    auto [x, y] = minmax(j, k);
                    if (make_pair(temp1[x], temp2[x]) > make_pair(cost, y)) {
                        temp1[x] = cost;
                        temp2[x] = y;
                    }
                };
                update(i, dp2[i]);
                update(dp2[i], i);
            }
        }

        dp1 = temp1;
        dp2 = temp2;
    }
    cout << *min_element(dp1.begin(), dp1.end());
}