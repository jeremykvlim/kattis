#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    auto lis_len = [&]() {
        vector<int> lis;
        for (int xi : x) {
            auto it = lower_bound(lis.begin(), lis.end(), xi);
            if (it != lis.end()) *it = xi;
            else lis.emplace_back(xi);
        }
        return lis.size();
    };
    int inc = lis_len();
    reverse(x.begin(), x.end());
    int dec = lis_len();
    cout << max(inc, dec);
}
