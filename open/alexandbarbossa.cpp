#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned k, m, n;
    cin >> k >> m >> n;
    
    auto r = k % (m + n);
    cout << (r >= n || (r / m) & 1 ? "Alex" : "Barbossa");
}
