#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> batteries;
    if (n < 6) {
        batteries.emplace_back(1, 3);
        batteries.emplace_back(1, 4);
        batteries.emplace_back(2, 3);
        batteries.emplace_back(2, 4);
        for (int i = 1; i <= n; i += 2) batteries.emplace_back(i, i + 1);
    } else {
        batteries.emplace_back(1, 2);
        batteries.emplace_back(1, 3);
        batteries.emplace_back(2, 3);
        batteries.emplace_back(4, 5);
        batteries.emplace_back(4, 6);
        batteries.emplace_back(5, 6);
        for (int i = 7; i <= n; i += 2) batteries.emplace_back(i, i + 1);
    }

    string s;
    for (auto [i, j] : batteries) {
        cout << i << " " << j << "\n" << flush;
        cin >> s;

        if (s == "Ljos!") exit(0);
    }
}
