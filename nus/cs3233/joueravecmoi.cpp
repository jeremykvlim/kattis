#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> values;
    for (int i = 0; i < n; i += 2) {
        int a1, b1, a2, b2;
        cin >> a1 >> b1 >> a2 >> b2;

        values.emplace_back(max(b1, b2));
        values.emplace_back(a1 + a2 - values.back());
    }
    sort(values.rbegin(), values.rend());

    auto sum = 0LL;
    for (int v : values) cout << (sum += v) << " ";
}