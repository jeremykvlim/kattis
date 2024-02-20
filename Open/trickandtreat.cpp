#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> requests(n);
    for (int &r : requests) cin >> r;

    int fewest = INT_MAX;
    for (int i = m - 9; i <= m; i++) {
        int candies = i, thrown = 0;
        for (int j = 0; j < n; j++)
            if (requests[j] > candies) thrown++;
            else candies -= requests[j];

        fewest = min(fewest, thrown);
    }
    
    cout << fewest;
}
