#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, k, c;
    cin >> m >> k >> c;
    cin.ignore();

    vector<vector<int>> C(26, vector<int>(26));
    for (int i = 0; i <= 25; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++) C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
    }

    string line;
    vector<int> keys;
    int id = 0, m1 = -1, m2 = 0, l = 0;
    while (getline(cin, line)) {
        line[0] = line.back() = ' ';

        stringstream ss(line);
        int m3 = 0, note;
        while (ss >> note) m3 |= 1 << note;

        keys.emplace_back(-1);
        int r = keys.size() - 1;
        auto update = [&]() {
            int p = popcount((unsigned) m2);
            if (p < k && k < m) return;

            id = 0;
            m1 = p == k ? m2 : (1 << m) - 1;
            for (int i = 0, prev = -1, count = 0; i < m; i++)
                if ((m1 >> i) & 1) {
                    for (int j = prev + 1; j < i; j++) id += C[m - j - 1][k - count - 1];
                    prev = i;
                    count++;
                }

            for (int i = l; i <= r; i++) keys[i] = id;
        };

        if (~m1) {
            if ((m1 | m3) == m1) keys[r] = id;
            else {
                l = r;
                m1 = -1;
                m2 = m3;
                update();
            }
        } else {
            m2 |= m3;
            if (popcount((unsigned) m2) > k) {
                l = r;
                m2 = m3;
            }
            update();
        }
    }

    for (int id : keys)
        if (!~id) cout << "?\n";
        else cout << id << "\n";
}
