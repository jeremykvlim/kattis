#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    int diff = 0;
    auto moves = 0LL;
    for (int i = 0; i < n; i++) {
        if (s[i] == '0') i & 1 ? diff-- : diff++;
        moves += abs(diff);
    }

    cout << (diff ? "impossible" : to_string(moves));
}
