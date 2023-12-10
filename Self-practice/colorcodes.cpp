#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p;
    cin >> n >> p;
    
    vector<bool> palette(n + 1, false);
    while (p--) {
        int s;
        cin >> s;
        palette[s] = true;
    }

    vector<vector<bool>> moves = {vector<bool>(n)};
    for (int i = 0; i < 1 << n; i++) {
        vector<bool> bits(n, false);
        for (int j = 0; j < n; j++) bits[j] = (i >> j) & 1;
        if (palette[count(bits.begin(), bits.end(), true)]) moves.emplace_back(bits);
    }

    vector<vector<bool>> matrix = moves;
    for (int r = 0; r < n; r++) {
        bool possible = false;
        for (int i = r; i < matrix.size(); i++)
            if (matrix[i][r]) {
                swap(matrix[i], matrix[r]);
                swap(moves[i], moves[r]);
                for (int j = i + 1; j < matrix.size(); j++)
                    if (matrix[j][r])
                        for (int k = 0; k < n; k++) matrix[j][k] = matrix[j][k] != matrix[r][k];
                possible = true;
                break;
            }
        
        if (!possible) {
            cout << "impossible" << endl;
            exit(0);
        }
    }

    vector<vector<bool>> code = {vector<bool>(n)};
    moves.resize(n);
    for (auto m : moves)
        for (int i = code.size() - 1; i >= 0; i--) {
            vector<bool> line(n);
            for (int j = 0; j < n; j++) line[j] = code[i][j] != m[j];
            code.emplace_back(line);
        }

    for (auto &line : code) {
        for (bool b : line) cout << b;
        cout << "\n";
    }
}
