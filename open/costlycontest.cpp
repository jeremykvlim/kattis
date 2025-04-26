#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, t;
    cin >> n >> m >> k >> t;

    vector<int> slowness(n);
    for (int &s : slowness) cin >> s;

    bitset<(int) 1e5> solvable;
    solvable[0] = true;
    for (int i = 0; i < m; i++) {
        int d;
        cin >> d;

        solvable |= solvable << d;
    }

    int total = t / *min_element(slowness.begin(), slowness.end()), threshold = INT_MAX;
    while (~total && !solvable[total]) total--;
    if (total) threshold = t / total + 1;

    cout << max(k, (int) count_if(slowness.begin(), slowness.end(), [&](int s) { return s < threshold; }));
}
