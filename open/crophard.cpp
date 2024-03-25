#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int X = 1; X <= t; X++) {
        int n, a, b, c, d, x, y, m;
        cin >> n >> a >> b >> c >> d >> x >> y >> m;

        vector<vector<long long>> coords(3, vector<long long>(3, 0));
        coords[x % 3][y % 3]++;
        for (int i = 1; i <= n - 1; i++) {
            x = ((long long) a * x + b) % m;
            y = ((long long) c * y + d) % m;
            coords[x % 3][y % 3]++;
        }

        auto triangles = 0LL;
        for (int i = 0; i < 9; i++) triangles += coords[i / 3][i % 3] * (coords[i / 3][i % 3] - 1) * (coords[i / 3][i % 3] - 2) / 6;

        for (int i = 0; i < 9; i++)
            for (int j = i + 1; j < 9; j++)
                for (int k = j + 1; k < 9; k++)
                    if (!((i / 3 + j / 3 + k / 3) % 3) && !((i % 3 + j % 3 + k % 3) % 3))
                        triangles += coords[i / 3][i % 3] * coords[j / 3][j % 3] * coords[k / 3][k % 3];

        cout << "Case #" << X << ": " << triangles << "\n";
    }
}
