#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> weight(n), lis(n, 1), lds(n, 1);
    for (auto &w : weight) cin >> w;

    int cars = 0;
    for (int i = n - 1; ~i; i--) {
        for (int j = i + 1; j < n; j++) {
            if (weight[j] > weight[i]) lis[i] = max(lis[i], lis[j] + 1);
            else if (weight[j] < weight[i]) lds[i] = max(lds[i], lds[j] + 1);
        }
        cars = max(cars, lis[i] + lds[i] - 1);
    }
    
    cout << cars;
}
