#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k = 25e4;
    cout << k << "\n";

    for (int l = 1; l < k; l *= 100)
        for (int i = 1; i < 100; i++)
            for (int j = k - l * 100; j >= 0; j -= l * 100) cout << i * l + j << " " << l << "\n";
    for (int y = 1e4; y <= k; y += 1e4) cout << y << " " << 1e4 << "\n";
}