#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> start(n), result(n);
    for (int &id : start) cin >> id;
    for (int &id : result) cin >> id;

    auto dist = [&](const vector<int> &v, int i, int j) {
        return (find(v.begin(), v.end(), i) - find(v.begin(), v.end(), j) + n) % n;
    };

    auto leap = [&](int b) {
        cout << b << "\n";
        for (int i = find(start.begin(), start.end(), b) - start.begin(); b; b--, ++i %= n) swap(start[i], start[(i + 1) % n]);
    };

    for (int i = n - 3; i; i--) {
        int j = i + 1;
        for (int k = i + 1; k < n; k++)
            if (dist(result, i, j) > dist(result, i, k)) j = k;

        int g = __gcd(n - 1, i);
        while (g != 1) {
            if (!((dist(start, i, j) - 1) % g)) break;
            while (dist(start, i, 1) != 1) leap(1);
            leap(i);
        }
        while (dist(start, i, j) != 1) leap(i);
    }
    while (dist(start, 1, n) != dist(result, 1, n)) leap(n);
}
