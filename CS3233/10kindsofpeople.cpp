#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int r, c;
    cin >> r >> c;
    vector<string> world(r);
    for (auto &row : world) cin >> row;

    vector<int> sets(r * c), rank(r * c, 0);
    iota(sets.begin(), sets.end(), 0);

    for (int i = 0; i < r; i++) 
        for (int j = 0; j < c; j++) {
            if (world[i][j] != '0' && world[i][j] != '1') continue;
            if (i > 0 && world[i][j] == world[i - 1][j]) {
                int curr = find(i * c + j, sets), prev = find((i - 1) * c + j, sets);
                if (curr != prev) {
                    sets[rank[curr] < rank[prev] ? curr : prev] = rank[curr] < rank[prev] ? prev : curr;
                    rank[curr] = rank[curr] == rank[prev] ? rank[curr]++ : rank[curr];
                }
            }
            if (j > 0 && world[i][j] == world[i][j - 1]) {
                int i_set = find(i * c + j, sets), j_set = find(i * c + j - 1, sets);
                if (i_set != j_set) {
                    sets[rank[i_set] < rank[j_set] ? i_set : j_set] = rank[i_set] < rank[j_set] ? j_set : i_set;
                    rank[i_set] = rank[i_set] == rank[j_set] ? rank[i_set]++ : rank[i_set];
                }
            }
        }
    
    int n;
    cin >> n;
    
    while (n--) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        
        if (find((r1 - 1) * c + (c1 - 1), sets) == find((r2 - 1) * c + (c2 - 1), sets)) {
            cout << (world[r1 - 1][c1 - 1] == '0' ? "binary\n" : "decimal\n");
        } else cout << "neither\n";
    }
}
