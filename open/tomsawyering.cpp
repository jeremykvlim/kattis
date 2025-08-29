#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n);
    for (int &xi : x) cin >> xi;

    auto area = 0LL;
    for (int l = 0, r = n - 1; l < r;) {
        area = max(area, (long long) (r - l) * min(x[l], x[r]));
        if (x[l] < x[r]) l++;
        else r--;
    }
    cout << area;
}
