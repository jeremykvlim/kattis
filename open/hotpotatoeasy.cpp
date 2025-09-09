#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> players(n);
    for (auto &name : players) cin >> name;

    vector<int> toys(k);
    for (int &l : toys) cin >> l;

    int pos = 0;
    for (int m = 2; m <= n; m++) pos = (pos + toys[(n - m) % k] + 1) % m;
    cout << players[pos];
}
