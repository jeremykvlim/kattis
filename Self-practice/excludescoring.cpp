#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> points = {100, 75, 60, 50, 45, 40, 36, 32, 29, 26, 24, 22, 20, 18, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    vector<vector<int>> contests(m, vector<int>(n - 1));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n - 1; j++) cin >> contests[i][j];
        
        sort(contests[i].rbegin(), contests[i].rend());
        contests[i].resize(4);
    }

    int rank = 1;
    for (int i = 1; i < m; i++)
        if (accumulate(contests[i].begin(), contests[i].end(), 0) > accumulate(contests[0].begin(), contests[0].end(), 0)) rank++;

    vector<int> diff;
    for (int i = 1; i < m; i++)
        if (accumulate(contests[i].begin(), contests[i].end(), 0) <= accumulate(contests[0].begin(), contests[0].end(), 0))
            diff.emplace_back(accumulate(contests[0].begin(), contests[0].end(), 0) - accumulate(contests[i].begin(), contests[i].begin() + 3, 0) + 1);
    sort(diff.begin(), diff.end());

    int l = 0, r = diff.size() + 1, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        int contestants = 0, total = 0;
        for (int i = 0; i < mid; i++) {
            contestants++;
            total += 1 + (mid - i <= 30 ? points[mid - i - 1] : 0);
            if ((total + contestants - 1) / contestants >= diff[i]) total = contestants = 0;
        }

        if (contestants) r = mid;
        else l = mid;
    }

    cout << rank + l;
}
