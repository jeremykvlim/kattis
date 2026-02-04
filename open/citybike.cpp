#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d, c;
    cin >> n >> d >> c;

    vector<int> bikes(n);
    for (int &bi : bikes) cin >> bi;

    int l = *ranges::partition_point(views::iota(0, d + 1), [&](int b) {
                int cap = c;
                for (int bi : bikes) {
                    if (b > bi) cap -= b - bi;
                    else cap = min(cap + bi - b, c);
                    if (cap < 0) return false;
                }
                return true;
            }) - 1;
    cout << *ranges::partition_point(views::iota(l, d + 1), [&](int b) {
                int cap = 0;
                for (int bi : bikes) {
                    if (bi > b) cap += bi - b;
                    else cap = max(cap - b + bi, 0);
                    if (cap > c) return true;
                }
                return false;
            }) - l;
}
