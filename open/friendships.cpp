#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> toys(n + 1, 0), children(51, 0);
    vector<vector<int>> adj_list(n + 1), count(n + 1, vector<int>(51, 0));
    children[0] = n;
    while (q--) {
        char c;
        int i;
        cin >> c >> i;

        if (c == 'F') {
            int j;
            cin >> j;

            adj_list[i].emplace_back(j);
            adj_list[j].emplace_back(i);
            count[i][toys[j]]++;
            count[j][toys[i]]++;
        } else if (c == 'A') {
            int k = toys[i]++;
            children[k]--;
            children[k + 1]++;
            for (int j : adj_list[i]) {
                count[j][k]--;
                count[j][k + 1]++;
            }
        } else {
            for (int k = 50; ~k; k--)
                if (children[k] - count[i][k] > (toys[i] == k)) {
                    cout << k << "\n";
                    goto next;
                }
            cout << "-1\n";
            next:;
        }
    }
}
