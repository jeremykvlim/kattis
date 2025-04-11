#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> a{0};
    int m;
    cin >> a[0] >> m;

    vector<bool> seen(m + 1, false);
    seen[0] = seen[a[0]] = true;
    int n = 1;
    for (int d = 1; !seen[m]; n++) {
        while (seen[d]) d++;
        
        if (a[n - 1] + d <= m) seen[a[n - 1] + d] = true;
        for (int ai : a)
            if (a[n - 1] + d - ai <= m) seen[a[n - 1] + d - ai] = true;
        a.emplace_back(a[n - 1] + d);
    }
    cout << n;
}
