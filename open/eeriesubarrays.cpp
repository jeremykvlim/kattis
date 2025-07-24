#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> p(n);
    for (int &pi : p) cin >> pi;

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return p[i] < p[j]; });

    int size = ceil(sqrt(n));
    vector<int> offset(n, n);
    vector<vector<int>> blocks(size, vector<int>(2 * n + 1, 0));
    for (int i = 0; i < n; i++) blocks[i / size][offset[i] -= i]++;

    auto scary = 0LL;
    vector<int> lazy(size, 0);
    for (int i : indices) {
        int b = i / size;

        for (int j = i; j < min(n, (b + 1) * size); j++) {
            blocks[b][offset[j]]--;
            blocks[b][offset[j] + 2]++;
            offset[j] += 2;
        }

        int pref = offset[i] + lazy[b];
        for (int block = b + 1; block <= (n - 1) / size; block++) {
            lazy[block] += 2;
            if (pref >= lazy[block]) scary += blocks[block][pref - lazy[block]];
        }

        for (int j = i; j < min(n, (b + 1) * size); j++)
            if (offset[j] + lazy[b] == pref) scary++;
    }
    cout << scary;
}
