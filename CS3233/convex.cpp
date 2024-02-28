#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s = 1e5;
    vector<pair<int, int>> coords;
    for (int i = 1; i < 500; i++)
        for (int j = 1; j < 500; j++)
            if (__gcd(i, j) == 1) coords.emplace_back(i, j);
    sort(coords.begin(), coords.end(), [](auto p1, auto p2) {return p1.first + p1.second < p2.first + p2.second;});
    while (coords.size() > s) coords.pop_back();
    sort(coords.begin(), coords.end(), [](auto p1, auto p2) {return p1.second * p2.first < p1.first * p2.second;});

    int n;
    cin >> n;

    vector<int> dx{1, -1, -1, 1}, dy{1, 1, -1, -1};
    int x = 2e7, y = 0;
    cout << x << " " << y << "\n";
    for (int i = 0; i < n - 1; i++) {
        int j = i / s, k = j & 1 ? s - 1 - i % s : i % s;

        x += coords[k].first * dx[j];
        y += coords[k].second * dy[j];
        cout << x << " " << y << "\n";
    }
}
