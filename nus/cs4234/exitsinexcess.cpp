#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> half1, half2;
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;

        u < v ? half1.emplace_back(i) : half2.emplace_back(i);
    }

    if (half1.size() > half2.size()) swap(half1, half2);
    cout << half1.size() << "\n";
    for (int i : half1) cout << i << "\n";
}
