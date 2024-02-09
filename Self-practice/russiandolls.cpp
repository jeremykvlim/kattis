#include <bits/stdc++.h>
using namespace std;

struct Doll {
    int h, d, w;

    bool operator<(const Doll &doll) const {
        int weight = -doll.w;
        return tie(h, d, w) < tie(doll.h, doll.d, weight);
    }
};

bool arrange(int i, int j, int count, vector<Doll> &dolls, vector<vector<vector<bool>>> &dp, bool set) {
    int k = max(i, j);
    if (2 * count > dolls.size() - 1 || 2 * (k - count) > dolls.size() - 1 || dp[i][j][count]) return false;
    if (k++ == dolls.size() - 1) return true;

    if ((dolls[k].h - 2 * dolls[k].w >= dolls[i].h && dolls[k].d - 2 * dolls[k].w >= dolls[i].d)
        && arrange(k, j, count + 1, dolls, dp, set)) {
        if (set) cout << dolls[k].h << " " << dolls[k].d << " " << dolls[k].w << "\n";
        return true;
    }

    if ((dolls[k].h - 2 * dolls[k].w >= dolls[j].h && dolls[k].d - 2 * dolls[k].w >= dolls[j].d)
        && arrange(i, k, count, dolls, dp, set)) {
        if (!set) cout << dolls[k].h << " " << dolls[k].d << " " << dolls[k].w << "\n";
        return true;
    }

    dp[i][j][count] = true;
    return false;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<Doll> dolls(2 * n + 1);
        for (int i = 1; i <= 2 * n; i++) cin >> dolls[i].h >> dolls[i].d >> dolls[i].w;
        sort(dolls.begin(), dolls.end());


        vector<vector<vector<bool>>> dp(2 * n + 1, vector<vector<bool>>(2 * n + 1, vector<bool>(2 * n + 1, false)));
        arrange(0, 0, 0, dolls, dp, false);
        cout << "-\n";
        arrange(0, 0, 0, dolls, dp, true);
        cout << "\n";
    }
}
