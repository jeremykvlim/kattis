#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> div(n, 1);
    for (int i = 2; i <= n; i++)
        for (int j = i - 1; j < n; j += i) div[j]++;

    for (int d : div) cout << d << "\n";
}
