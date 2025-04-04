#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> degree(n, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        degree[a - 1]++;
        degree[b - 1]++;
    }

    vector<int> count(*max_element(degree.begin(), degree.end()) + 1, 0);
    for (int d : degree) count[d]++;

    int d = 0, e = m * 2;
    while (d < n) {
        e -= d * count[d];
        n -= count[d];
        d++;
    }
    e += (d - n) * (d - 1) - d * (d - 1) / 2;
    cout << (e != m ? 0 : (d != n ? count[d - 1] + 1 : 1));
}
