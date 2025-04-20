#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, c;
    cin >> n >> k >> c;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto temp = a;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    for (int &ai : a) ai = lower_bound(temp.begin(), temp.end(), ai) - temp.begin();

    a.resize(2 * n);
    for (int i = n; i < 2 * n; i++) a[i] = a[i - n];

    int m = 2 * n;
    vector<int> distinct(m + 2, m + 1), freq(temp.size(), 0);
    vector<vector<int>> lift(__lg(n) + 1, vector<int>(m + 2, m + 1));
    for (int l = 1, r = 1, count = 0; l <= m; l++) {
        while (r <= m && count < c) {
            if (!freq[a[r - 1]]++) count++;
            r++;
        }

        if (count >= c) distinct[l] = r;
        else distinct[l] = m + 1;

        if (!--freq[a[l - 1]]) count--;
        lift[0][l] = min(m+1, max(distinct[l], l + k));
    }

    for (int j = 1; j <= __lg(n); j++)
        for (int i = 1; i <= m + 1; i++)
            lift[j][i] = min(m + 1, lift[j - 1][lift[j - 1][i]]);

    int companies = 0;
    for (int l = 1; l <= n; l++) {
        int count = 0;
        for (int i = __lg(n), j = l; ~i; i--)
            if (l + n >= lift[i][j]) {
                j = lift[i][j];
                count += 1 << i;
            }
        companies = max(companies, count);
    }
    cout << companies;
}
