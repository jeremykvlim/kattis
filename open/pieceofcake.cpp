#include <bits/stdc++.h>
using namespace std;

double cross(pair<double, double> i, pair<double, double> j) {
    return (i.first * j.second) - (i.second * j.first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<double, double>> vertices(n);
    for (auto &[x, y]: vertices) cin >> x >> y;

    vector<vector<double>> ways(n, vector<double>(n, -1));
    double lg_nck = 0, lg_kci = 0;
    for (int i = 0; i < k; i++) lg_nck += log10(n - i) - log10(i + 1);
    for (int i = 0; i < n - k + 1; i++) {
        for (int j = 0; j < n; j++) ways[j][(n - k + 1 - i + j) % n] = lg_kci;
        lg_kci += log10(k + i - 1) - log10(i + 1);
    }

    double area = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (ways[i][j] != -1)
                area += pow(10, ways[i][j] - lg_nck) * cross(vertices[i], vertices[j]);

    cout << fixed << setprecision(6) << abs(area) / 2;
}
