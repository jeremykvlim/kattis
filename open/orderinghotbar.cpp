#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto temp = a;
    sort(temp.begin(), temp.end());
    unordered_map<int, int> indices;
    for (int i = 0; i < n; i++) indices[temp[i]] = i;
    for (int &ai : a) ai = indices[ai];

    vector<int> dp(n, INT_MIN);
    for (int ai : a) {
        dp[ai] = 1;
        if (ai) dp[ai] = max(dp[ai], dp[ai - 1] + 1);
    }

    cout << n - *max_element(dp.begin(), dp.end());
}
