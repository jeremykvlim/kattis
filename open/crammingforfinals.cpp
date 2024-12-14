#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c, d, n;
    cin >> r >> c >> d >> n;

    vector<pair<int, int>> students(n);
    for (auto &[i, j] : students) {
        cin >> i >> j;
        i--;
        j--;
    }

    if (r > c) {
        swap(r, c);
        for (auto &[i, j] : students) swap(i, j);
    }

    vector<int> col(d + 1);
    for (int i = 0; i <= d; i++)
        for (int j = 0; j <= d; j++)
            if (i * i + j * j <= d * d) col[i] = j;

    int count = n;
    for (int i1 = 0; i1 < r; i1++) {
        map<long long, int> freq{{0, 0}, {c, 0}};

        for (auto [i2, j2] : students) {
            if (abs(i1 - i2) > d) continue;

            if (i1 == i2) {
                freq[j2] += n + 1;
                freq[j2 + 1] -= n + 1;
            }

            int j1 = col[abs(i1 - i2)];
            freq[max(j2 - j1, 0)]++;
            freq[min(j2 + j1, c - 1) + 1]--;
        }

        int sum = 0;
        for (auto [j, f] : freq) {
            sum += f;
            if (j < c) count = min(count, sum);
        }
        if (!count) break;
    }
    cout << count;
}
