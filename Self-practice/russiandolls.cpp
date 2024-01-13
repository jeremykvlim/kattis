#include <bits/stdc++.h>
using namespace std;

struct Doll {
    int h, d, w;

    bool operator<(const Doll &doll) const {
        int weight = -doll.w;
        return tie(h, d, w) < tie(doll.h, doll.d, weight);
    }
};

bool arrange(int doll1, int doll2, int count, vector<Doll> &dolls, bool set, vector<vector<vector<bool>>> &dp) {
    int bigger = max(doll1, doll2);
    if (2 * count > dolls.size() - 1 || 2 * (bigger - count) > dolls.size() - 1 || dp[doll1][doll2][count]) return false;
    if (bigger++ == dolls.size() - 1) return true;
    
    if ((dolls[bigger].h - 2 * dolls[bigger].w >= dolls[doll1].h && dolls[bigger].d - 2 * dolls[bigger].w >= dolls[doll1].d)
        && arrange(bigger, doll2, count + 1, dolls, set, dp)) {
        if (set) cout << dolls[bigger].h << " " << dolls[bigger].d << " " << dolls[bigger].w << "\n";
        return true;
    }
    
    if ((dolls[bigger].h - 2 * dolls[bigger].w >= dolls[doll2].h && dolls[bigger].d - 2 * dolls[bigger].w >= dolls[doll2].d)
        && arrange(doll1, bigger, count, dolls, set, dp)) {
        if (!set) cout << dolls[bigger].h << " " << dolls[bigger].d << " " << dolls[bigger].w << "\n";
        return true;
    }
    
    dp[doll1][doll2][count] = true;
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
        arrange(0, 0, 0, dolls, false, dp);
        cout << "-\n";
        arrange(0, 0, 0, dolls, true, dp);
        cout << "\n";
    }
}
