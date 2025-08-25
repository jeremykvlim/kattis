#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> grid(h);
    for (auto &row : grid) cin >> row;

    int count = 0;
    vector<vector<int>> pref(h + 1, vector<int>(w + 1, 0));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            if (grid[i][j] == '#') count++;
            pref[i + 1][j + 1] = pref[i + 1][j] + pref[i][j + 1] - pref[i][j] + (grid[i][j] == '#');
        }

    int temp = count;
    vector<pair<int, int>> pfs;
    for (int p = 2; p * p <= count; p == 2 ? p++ : p += 2)
        if (!(count % p)) {
            int pow = 0;
            while (!(count % p)) {
                count /= p;
                pow++;
            }
            pfs.emplace_back(p, pow);
        }
    if (count > 1) pfs.emplace_back(count, 1);
    count = temp;

    int m = pfs.size();
    vector<int> possible;
    auto dfs = [&](auto &&self, int i = 0, int len = 1) -> void {
        if (i == m) {
            if (len <= min(h, w)) possible.emplace_back(len);
            return;
        }

        auto [pf, pow] = pfs[i];
        int pp = 1;
        for (pow /= 2; ~pow; pow--) {
            if (len * pp > min(h, w)) break;
            self(self, i + 1, len * pp);
            pp *= pf;
        }
    };
    dfs(dfs);

    if (possible.empty()) {
        cout << 1;
        exit(0);
    }
    sort(possible.rbegin(), possible.rend());

    int t = 1;
    vector<vector<int>> visited(h, vector<int>(w, 0));
    for (int len : possible) {
        if (len * len > count) continue;
        int painted = 0;
        for (int i = 0; i < h; i++)
            for (int j = 0; j < w; j++)
                if (grid[i][j] == '#')
                    if (visited[i][j] != t) {
                        if (i + len > h || j + len > w || pref[i + len][j + len] - pref[i][j + len] - pref[i + len][j] + pref[i][j] != len * len) goto next;
                        for (int k = i; k < i + len; k++)
                            for (int l = 0; l < len; l++) visited[k][j + l] = t;
                        painted += len * len;
                    }

        if (painted == count) {
            cout << len;
            exit(0);
        }
        next:;
        t++;
    }
    cout << 1;
}
