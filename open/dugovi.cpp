#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n + 1), b(n + 1), degree(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> a[i] >> b[i];

        degree[a[i]]++;
    }

    vector<int> sum(n + 1, 0);
    vector<bool> visited(n + 1, false);
    int money = 0;
    for (int i = 1; i <= n; i++)
        if (!degree[i] && !visited[i])
            for (int j = i, k = a[j]; !degree[j]; j = a[j], k = a[k]) {
                money += max(0, b[j] - sum[j]);
                sum[k] += b[j];
                visited[j] = true;
                degree[j] = 0;
                degree[k]--;
            }

    for (int i = 1; i <= n; i++)
        if (degree[i]) {
            int m = INT_MAX;
            for (int j = i, k = a[j]; degree[k]; j = k, k = a[k]) {
                int diff1 = max(0, b[k] - b[j] - sum[k]), diff2 = max(0, b[k] - sum[k]);
                money += diff1;
                m = min(m, diff2 - diff1);
                degree[k] = 0;
            }
            money += m;
        }

    cout << money;
}
