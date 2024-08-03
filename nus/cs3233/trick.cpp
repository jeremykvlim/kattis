#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double x;
    cin >> x;

    vector<int> list;
    for (int i = 1; i < 1e8; i++) {
        int j = 1;
        while (j <= i) j *= 10;

        if (abs(i * x - i * 10 / j - i * 10 % j) < 1e-7) list.emplace_back(i);
    }

    if (list.empty()) {
        cout << "No solution";
        exit(0);
    }

    for (int i : list) cout << i << "\n";
}
