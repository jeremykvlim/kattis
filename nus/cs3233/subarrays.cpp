#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    long long b;
    cin >> n >> k >> b;

    auto pref = 0LL;
    unordered_map<long long, int> seen{{0, 0}};
    pair<int, int> subarray{n, n};
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;

        pref += a - k;
        auto it = seen.find(pref - b);
        if (it != seen.end()) subarray = min(subarray, make_pair(it->second, i));
        seen.emplace(pref, i + 1);
    }

    if (subarray == make_pair(n, n)) cout << -1;
    else cout << subarray.first << " " << subarray.second;
}
