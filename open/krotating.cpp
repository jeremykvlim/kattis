#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    int size = ceil(sqrt(m));
    vector<vector<int>> blocks(size, vector<int>(n));
    for (int i = 0; i < size; i++) iota(blocks[i].begin(), blocks[i].end(), 0);

    vector<vector<int>> teachers(m + size);
    while (q--) {
        int c;
        cin >> c;

        if (!c) {
            int k, x;
            cin >> k >> x;

            int b = --x / size;
            for (int i = (b + 1) * size - 1; i >= x; i--)
                if (teachers[i].size() > 1)
                    for (int j = teachers[i].size() - 1; j; j--)
                        swap(blocks[b][teachers[i][j - 1]], blocks[b][teachers[i][j]]);

            while (k--) {
                int p;
                cin >> p;

                teachers[x].emplace_back(p - 1);
            }

            for (int i = x; i < (b + 1) * size; i++)
                if (teachers[i].size() > 1)
                    for (int j = 1; j < teachers[i].size(); j++)
                        swap(blocks[b][teachers[i][j - 1]], blocks[b][teachers[i][j]]);
        } else {
            int d, x;
            cin >> d >> x;

            for (d--, x--; (x + 1) % size; x--)
                for (int i = 0; i < teachers[x].size(); i++)
                    if (d == teachers[x][i]) {
                        d = teachers[x][(i + 1) % teachers[x].size()];
                        break;
                    }

            for (int i = (x + 1) / size - 1; ~i; i--) d = blocks[i][d];
            cout << d + 1 << "\n";
        }
    }
}
