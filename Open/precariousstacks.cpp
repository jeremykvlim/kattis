#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    map<int, long long> heights{{INT_MAX, 0}};
    auto y = 0LL;
    while (n--) {
        int x, s;
        cin >> x >> s;

        auto it = heights.lower_bound(x);
        auto h = it->second, curr = it->second;
        while (it->first < x + s - 1) {
            it = heights.erase(it);
            curr = max(curr, it->second);
        }

        heights[x + s - 1] = curr + s;
        heights.try_emplace(x - 1, h);

        y = max(y, curr + s);
        cout << y << "\n";
    }
}
