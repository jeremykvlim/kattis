#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p;
    cin >> p;

    while (p--) {
        int k;
        cin >> k;

        vector<int> heights(20);
        int steps = 0;
        for (int i = 0; i < 20; i++) {
            cin >> heights[i];

            for (int j = 0; j < i; j++) steps += heights[j] > heights[i];
        }

        cout << k << " " << steps << "\n";
    }
}
