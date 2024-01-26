#include <bits/stdc++.h>
using namespace std;

void update(vector<string> &matrix, vector<vector<int>> &above, vector<vector<int>> &below, int c, int R) {
    int last = -1;
    for (int r = 0; r < R; r++) {
        if (matrix[r][c] == 'x') last = r;
        above[r][c] = last;
    }
    
    last = -1;
    for (int r = R - 1; ~r; r--) {
        if (matrix[r][c] == 'x') last = r;
        below[r][c] = last;
    }
}

int dist(vector<vector<int>> &above, vector<vector<int>> &below, int r, int s, int S) {
    int d = INT_MAX;
    for (int t = 0; t < S; t++) {
        if (above[r][t] != -1) d = min(d, (r - above[r][t]) * (r - above[r][t]) + (s - t) * (s - t));
        if (below[r][t] != -1) d = min(d, (r - below[r][t]) * (r - below[r][t]) + (s - t) * (s - t));
    }
    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, S, G;
    cin >> R >> S;

    vector<string> matrix(R);
    for (auto &r : matrix) cin >> r;
    
    vector<vector<int>> above(R, vector<int>(S, -1)), below(R, vector<int>(S, -1));
    for (int s = 0; s < S; s++) update(matrix, above, below, s, R);

    cin >> G;
    while (G--) {
        int r, s;
        cin >> r >> s;
        cout << dist(above, below, --r, --s, S) << "\n";
        
        matrix[r][s] = 'x';
        update(matrix, above, below, s, R);
    }
}
