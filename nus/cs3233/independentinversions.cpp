#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> b(n);
    for (int &bi : b) cin >> bi;
    
    int count = 0;
    auto dfs = [&](auto &&self, int x, int y, int d = 0) {
        if (d == k - 1) {
            count += b[x] > b[y];
            return;
        } 
        
        for (int i = 0; i < 3; i++) 
            for (int j = 0; j < 3; j++) 
                if (i != j) self(self, x * 3 + i, y * 3 + j, d + 1);
    };
    dfs(dfs, 0, 1);
    dfs(dfs, 0, 2);
    dfs(dfs, 1, 2);
    cout << count;
}
