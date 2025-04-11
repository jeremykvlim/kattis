#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> a(1);
    int m;
    cin >> a[0] >> m;

    dynamic_bitset<> bs(m + 1);
    bs.set();
    bs[0] = bs[a[0]] = false;

    int n = 1;
    for (int prev = 0, d; bs[m]; prev = d, n++) {
        d = bs.find_next(prev);
        bs[d] = false;
        if (a[n - 1] + d <= m) bs[a[n - 1] + d] = false;
        for (int ai : a)
            if (a[n - 1] + d - ai <= m) bs[a[n - 1] + d - ai] = false;

        a.emplace_back(a[n - 1] + d);
    }

    cout << n;
}
