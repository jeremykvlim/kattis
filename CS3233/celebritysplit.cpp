#include <bits/stdc++.h>
using namespace std;

void dfs(int i, int jack, int jill, int sold, int n, vector<int> &mansions, vector<int> &suff, int &value) {
    if (sold >= value || abs(jack - jill) > suff[i]) return;
    if (jack == jill) value = min(value, sold + suff[i]);
    if (i == n) return;
    
    dfs(i + 1, jack + mansions[i], jill, sold, n, mansions, suff, value);
    if (jack != jill && jack) dfs(i + 1, jack, jill + mansions[i], sold, n, mansions, suff, value);
    dfs(i + 1, jack, jill, sold + mansions[i], n, mansions, suff, value);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> mansions(n), suff(n + 1, 0);
        for (int &v : mansions) cin >> v;
        sort(mansions.rbegin(), mansions.rend());
        
        for (int i = n - 1; ~i; i--) suff[i] = suff[i + 1] + mansions[i];
        
        int value = INT_MAX;
        dfs(0, 0, 0, 0, n, mansions, suff, value);
        cout << value << "\n";
    }
}
