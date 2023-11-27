#include <bits/stdc++.h>
using namespace std;

struct Doll {
    int h, d, w;

    bool operator<(const Doll &doll) const {
        if (h != doll.h) return h < doll.h;
        if (d != doll.d) return d < doll.d;
        return w > doll.w;
    }
};

bool arrange(int doll1, int doll2, int counter, vector<Doll> &dolls, bool set, vector<vector<vector<bool>>> &dp) {
    int bigger = max(doll1, doll2);
    if (counter > (dolls.size() - 1) / 2 || bigger - counter > (dolls.size() - 1) / 2 || dp[doll1][doll2][counter]) return false;
    if (bigger++ == dolls.size() - 1) return true;
    if ((dolls[bigger].h - 2 * dolls[bigger].w >= dolls[doll1].h && dolls[bigger].d - 2 * dolls[bigger].w >= dolls[doll1].d)
        && arrange(bigger, doll2, counter + 1, dolls, set, dp)) {
        if (set) cout << dolls[bigger].h << " " << dolls[bigger].d << " " << dolls[bigger].w << "\n";
        return true;
    }
    if ((dolls[bigger].h - 2 * dolls[bigger].w >= dolls[doll2].h && dolls[bigger].d - 2 * dolls[bigger].w >= dolls[doll2].d)
        && arrange(doll1, bigger, counter, dolls, set, dp)) {
        if (!set) cout << dolls[bigger].h << " " << dolls[bigger].d << " " << dolls[bigger].w << "\n";
        return true;
    }
    dp[doll1][doll2][counter] = true;
    return false;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    while (cin >> n && n) {
        n <<= 1;
        vector<Doll> dolls(n + 1);
        vector<vector<vector<bool>>> memo(n + 1, vector<vector<bool>>(n + 1, vector<bool>(n + 1, false)));
        for (int i = 1; i <= n; i++) cin >> dolls[i].h >> dolls[i].d >> dolls[i].w;
        sort(dolls.begin(), dolls.end());

        arrange(0, 0, 0, dolls, 0, memo);
        cout << "-\n";
        arrange(0, 0, 0, dolls, 1, memo);
        cout << "\n";
    }
}
