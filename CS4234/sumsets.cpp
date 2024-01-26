#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> set(n);
    for (int &s : set) cin >> s;
    sort(set.begin(), set.end());

    unordered_map<int, pair<int, int>> sums;
    for (int a = 0; a < n; a++)
        for (int b = a + 1; b < n; b++) sums[set[a] + set[b]] = {a, b};

    for (int d = n - 1; ~d; d--)
        for (int c = 0; c < n; c++) {
            if (c == d) continue;

            if (sums.count(set[d] - set[c])) {
                auto [a, b] = sums[set[d] - set[c]];

                if (a != d && b != d && a != c && b != c) {
                    cout << set[d];
                    exit(0);
                }
            }
        }

    cout << "no solution";
}
