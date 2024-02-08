#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> sets(n);
    iota(sets.begin(), sets.end(), 0);

    unordered_map<int, int> nesw, nwse;
    int C = n;
    for (int i = 0; i < n; i++) {
        int r, c;
        cin >> r >> c;

        for (int j = 0; j <= 2; j++) {
            if (nesw.count(r + c + j - 1)) {
                int i_set = find(i, sets), j_set = find(nesw[r + c + j - 1], sets);
                if (i_set != j_set) C--;
                sets[i_set] = j_set;
            }

            if (nwse.count(r - c + j - 1)) {
                int i_set = find(i, sets), j_set = find(nwse[r - c + j - 1], sets);
                if (i_set != j_set) C--;
                sets[i_set] = j_set;
            }
        }

        nesw[r + c] = nwse[r - c] = i;
    }

    cout << C;
}
