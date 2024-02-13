#include <bits/stdc++.h>
using namespace std;

bool bron_kerbosch(int depth, vector<vector<bool>> &friends, int all, int some, int none, vector<vector<int>> &r, vector<vector<int>> &p, vector<vector<int>> &x, int &count) {
    if (!some && !none) count++;
    for (int i = 0, pivot = p[depth][0]; i < some; i++) {
        int v = p[depth][i]; 
        if (friends[pivot][v]) continue;
        
        r[depth + 1] = r[depth];
        r[depth + 1][all] = v;

        int next_some = 0, next_none = 0;
        for (int j = 0; j < some; j++)
            if (friends[v][p[depth][j]]) p[depth + 1][next_some++] = p[depth][j];
        for (int j = 0; j < none; j++)
            if (friends[v][x[depth][j]]) x[depth + 1][next_none++] = x[depth][j];
        if (!bron_kerbosch(depth + 1, friends, all + 1, next_some, next_none, r, p, x, count)) return false;
        
        p[depth][i] = 0;
        x[depth][none++] = v;
        if (count > 1e3) return false;
    }
    
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        vector<vector<bool>> friends(n + 2, vector<bool>(n + 2, false));
        vector<vector<int>> r(n + 2, vector<int>(n + 2, 0)), p(n + 2, vector<int>(n + 2 , 0)), x(n + 2, vector<int>(n + 2, 0));
        while (m--) {
            int a, b;
            cin >> a >> b;
            
            friends[a][b] = friends[b][a] = true;
        }
        iota(p[1].begin(), p[1].end(), 1);
        
        int count = 0;
        cout << (bron_kerbosch(1, friends, 0, n, 0, r, p, x, count) ? to_string(count) + "\n" : "Too many maximal sets of friends.\n");
    }
}
