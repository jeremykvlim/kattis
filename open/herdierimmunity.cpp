#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(m);
    for (int &ai : a) cin >> ai;
    sort(a.begin(), a.end());

    priority_queue<int> pq;
    for (int i = 1; i < m; i++) pq.emplace(a[i] - a[i - 1] - 1);

    int l = a[0] - 1, r = n - a[m - 1];
    vector<vector<int>> dp(4, vector<int>(2, -1e9));
    dp[0][0] = 0;
    for (int days = 0; !pq.empty(); days += 2) {
        int len = pq.top();
        pq.pop();

        vector<vector<int>> temp(4, vector<int>(2, -1e9));
        temp[0][0] = max(temp[0][0], dp[0][0] + max(0, len - 2 * days - 1));
        temp[0][1] = max(temp[0][1], dp[0][1] + max(0, len - 2 * (days - 1) - 1));
        if (len - 2 * days == 1) temp[0][1] = max(temp[0][1], dp[0][0] + 1);

        for (int i = 1; i <= 2; i++) {
            temp[i][0] = max(temp[i][0], dp[0][0] + max(0, (i & 1 ? l : r) - days) + max(0, len - 2 * (days + 1) - 1));
            temp[i][0] = max(temp[i][0], dp[i][0] + max(0, len - 2 * (days + 1) - 1));
            temp[i][1] = max(temp[i][1], dp[0][1] + max(0, (i & 1 ? l : r) - (days - 1)) + max(0, len - 2 * days - 1));
            temp[i][1] = max(temp[i][1], dp[i][1] + max(0, len - 2 * days - 1));
        }
        if (len - 2 * (days + 1) == 1) {
            temp[1][1] = max(temp[1][1], dp[1][0] + 1);
            temp[2][1] = max(temp[2][1], dp[2][0] + 1);
        }

        for (int j = 0; j < 2; j++) {
            temp[3][j] = max(temp[3][j], dp[0][j] + max(0, l - (days - j)) + max(0, r - (days + 1 - j)) + max(0, len - 2 * (days + 2 - j) - 1));
            temp[3][j] = max(temp[3][j], dp[0][j] + max(0, r - (days - j)) + max(0, l - (days + 1 - j)) + max(0, len - 2 * (days + 2 - j) - 1));
            temp[3][j] = max(temp[3][j], dp[1][j] + max(0, r - (days + 1 - j)) + max(0, len - 2 * (days + 2 - j) - 1));
            temp[3][j] = max(temp[3][j], dp[2][j] + max(0, l - (days + 1 - j)) + max(0, len - 2 * (days + 2 - j) - 1));
            temp[3][j] = max(temp[3][j], dp[3][j] + max(0, len - 2 * (days + 2 - j) - 1));
        }
        if (len - 2 * (days + 2) == 1) temp[3][1] = max(temp[3][1], dp[3][0] + 1);

        dp = temp;
    }

    int count = max(l + max(0, r - 1), max(0, l - 1) + r);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++) count = max(count, dp[i][j]);
    cout << count;
}
