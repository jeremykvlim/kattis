#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w, a, b;
    cin >> h >> w >> a >> b;

    vector<vector<int>> s(h + 1, vector<int>(w + 1)), pref(h + 1, vector<int>(w + 1));
    for (int i = 1; i <= h; i++)
        for (int j = 1; j <= w; j++) {
            cin >> s[i][j];

            pref[i][j] = pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1] + s[i][j];
        }

    int total = 0;
    vector<int> count(1e7 + 1, 0);
    for (int i = 1; i <= h; i++)
        for (int j = 1; j <= w; j++) {
            int area = i * j;
            if (a <= area && area <= b)
                for (int x = 1; x <= h - i + 1; x++)
                    for (int y = 1; y <= w - j + 1; y++) {
                        total++;
                        count[round((pref[x + i - 1][y + j - 1] - pref[x - 1][y + j - 1] - pref[x + i - 1][y - 1] + pref[x - 1][y - 1]) * 1e3 / area)]++;
                    }
        }

    bool odd = total & 1;
    total /= 2;
    total += odd;
    for (int i = 0; i <= 1e7; i++) {
        if (total > count[i]) {
            total -= count[i];
            continue;
        }

        if (total < count[i] || total == count[i] && odd) {
            cout << fixed << setprecision(3) << i / 1e3;
            exit(0);
        }

        int j = i + 1;
        for (; !count[j]; j++);
        cout << fixed << setprecision(3) << (i + (j - i) / 2) / 1e3;
        exit(0);
    }
}
