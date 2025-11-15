#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> indices(n + 1);
    for (int i = 0; i < n; i++) {
        int p;
        cin >> p;

        indices[p] = i;
    }

    while (m-- > 1) {
        vector<int> lcs;
        for (int _ = 0; _ < n; _++) {
            int pi;
            cin >> pi;

            pi = indices[pi];
            auto it = lower_bound(lcs.begin(), lcs.end(), pi);
            if (it == lcs.end()) lcs.emplace_back(pi);
            else *it = pi;
        }
        cout << lcs.size() << "\n";
    }
}
