#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e6 + 3;

long long cross(pair<int, int> i, pair<int, int> j, pair<int, int> k) {
    return (long long) (k.first - i.first) * (long long) (j.second - i.second) - (long long) (k.second - i.second) * (long long) (j.first - i.first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;
    sort(coords.begin(), coords.end());

    vector<vector<int>> count(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = i + 1; k < j; k++)
                if (cross(coords[i], coords[j], coords[k]) > 0) {
                    count[i][j]++;
                    count[j][i]++;
                }

    auto sum = 0LL;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            for (int k = j + 1; k < n; k++) {
                auto cross_product = cross(coords[i], coords[j], coords[k]);
                int c = count[i][j] + count[j][k] - count[k][i];
                if (cross_product < 0) {
                    cross_product = -cross_product;
                    c = -c - 1;
                }

                sum += cross_product * (c * 3 + n - 3 - c) % MODULO;
            }
    sum %= MODULO;
    if (sum & 1) sum += MODULO;

    cout << sum / 2;
}
