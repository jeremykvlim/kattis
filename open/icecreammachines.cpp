#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> c(n);
    for (int &ci : c) cin >> ci;

    vector<int> indices(m + 1, INT_MAX), next(n);
    for (int i = n - 1; ~i; i--) {
        next[i] = indices[c[i]];
        indices[c[i]] = i;
    }

    set<int> s;
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (s.count(i)) s.erase(i);
        else {
            count++;
            if (s.size() >= k) s.erase(*s.rbegin());
        }

        s.emplace(next[i]);
    }
    cout << count;
}
