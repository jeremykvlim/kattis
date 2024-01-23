#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    int total = 1, b = 0;
    while (k--) swap(total, b += total);

    cout << total - b << " " << b;
}
