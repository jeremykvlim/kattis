#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int most = 0;
    vector<int> count(2e6 + 1, 0);
    for (int _ = 0; _ < n; _++) {
        int m;
        cin >> m;

        most = max(most, m);
        count[m]++;
    }

    long long finalists = n;
    for (int i = 2; i <= most; i++) {
        auto c = 0LL;
        for (int j = i; j <= most; j += i) c += count[j];
        if (c >= 2) finalists = max(finalists, i * c);
    }
    cout << finalists;
}