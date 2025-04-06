#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        int tests = 0;
        for (int sum = 0; sum < n - 1; sum += ++tests);
        cout << tests << "\n";
    }
}
