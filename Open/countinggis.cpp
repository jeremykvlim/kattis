#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l;
    cin >> n >> l;

    vector<int> G(l);
    unordered_set<int> g;
    for (int i = 0; i < l; i++) {
        cin >> G[i];
        if (i && G[i - 1] >= G[i]) {
            cout << 0;
            exit(0);
        }
        g.insert(G[i]);
    }

    if (G.back() != n) {
        cout << 0;
        exit(0);
    }
    
    long long a = 1;
    int count = 0;
    for (int i = n; i; i--)
        if (!g.count(i)) a = a * (G.size() - (int)(lower_bound(G.begin(), G.end(), i) - G.begin()) + count++) % MODULO;

    cout << a;
}
