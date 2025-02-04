#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    void reroot() {
        for (int i = 0; i < sets.size(); i++) sets[i] = sets[find(i)];
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

struct Hash {
    size_t operator()(pair<int, int> p) const {
        auto h = hash<int>()(p.first);
        h ^= hash<int>()(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }

    size_t operator()(array<int, 3> a) const {
        auto h = hash<int>()(a[0]);
        h ^= hash<int>()(a[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= hash<int>()(a[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }

    size_t operator()(array<int, 4> a) const {
        auto h = hash<int>()(a[0]);
        h ^= hash<int>()(a[1]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= hash<int>()(a[2]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= hash<int>()(a[3]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, R;
    cin >> n >> R;

    vector<pair<int, int>> bombs;
    vector<vector<int>> indices(n, vector<int>(n, -1));
    vector<vector<char>> grid(n, vector<char>(n));
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++) {
            cin >> grid[r][c];

            if (grid[r][c] == '*') {
                bombs.emplace_back(r, c);
                indices[r][c] = bombs.size() - 1;
            }
        }

    vector<vector<int>> up(n, vector<int>(n)), down(n, vector<int>(n)), left(n, vector<int>(n)), right(n, vector<int>(n));
    for (int c = 0; c < n; c++) {
        int prev = -1;
        for (int r = 0; r < n; r++)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = up[r][c] = r;
            else up[r][c] = prev;

        prev = -1;
        for (int r = n - 1; ~r; r--)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = down[r][c] = r;
            else down[r][c] = (prev == -1 ? n : prev);
    }

    for (int r = 0; r < n; r++) {
        int prev = -1;
        for (int c = 0; c < n; c++)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = left[r][c] = c;
            else left[r][c] = prev;

        prev = -1;
        for (int c = n - 1; ~c; c--)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = right[r][c] = c;
            else right[r][c] = (prev == -1 ? n : prev);
    }

    int b = bombs.size();
    DisjointSets dsu(b);
    for (int r = 0; r < n; r++) {
        vector<int> row_bombs;
        auto join = [&]() {
            for (int i = 1; i < row_bombs.size(); i++) {
                int cl = bombs[row_bombs[i - 1]].second, cr = bombs[row_bombs[i]].second;
                if (cr - cl <= R) dsu.unite(row_bombs[i - 1], row_bombs[i]);
            }
        };
        for (int c = 0; c < n; c++)
            if (grid[r][c] == '#' && !row_bombs.empty()) {
                join();
                row_bombs.clear();
            } else if (grid[r][c] == '*') row_bombs.emplace_back(indices[r][c]);

        if (!row_bombs.empty()) join();
    }

    for (int c = 0; c < n; c++) {
        vector<int> col_bombs;
        auto join = [&]() {
            for (int i = 1; i < col_bombs.size(); i++) {
                int rl = bombs[col_bombs[i - 1]].first, rr = bombs[col_bombs[i]].first;
                if (rr - rl <= R) dsu.unite(col_bombs[i - 1], col_bombs[i]);
            }
        };
        for (int r = 0; r < n; r++)
            if (grid[r][c] == '#' && !col_bombs.empty()) {
                join();
                col_bombs.clear();
            } else if (grid[r][c] == '*') col_bombs.emplace_back(indices[r][c]);

        if (!col_bombs.empty()) join();
    }
    dsu.reroot();

    int stumps = 0;
    vector<vector<vector<int>>> stump_bombs(n, vector<vector<int>>(n));
    unordered_map<int, int> f1;
    unordered_map<pair<int, int>, int, Hash> f2;
    unordered_map<array<int, 3>, int, Hash> f3;
    unordered_map<array<int, 4>, int, Hash> f4;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            if (grid[r][c] == 'S') {
                stumps++;
                if (left[r][c] != -1 && c - left[r][c] <= R && grid[r][left[r][c]] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[r][left[r][c]]]);
                if (right[r][c] < n && right[r][c] - c <= R && grid[r][right[r][c]] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[r][right[r][c]]]);
                if (up[r][c] != -1 && r - up[r][c] <= R && grid[up[r][c]][c] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[up[r][c]][c]]);
                if (down[r][c] < n && down[r][c] - r <= R && grid[down[r][c]][c] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[down[r][c]][c]]);
                if (stump_bombs[r][c].empty()) continue;
                
                sort(stump_bombs[r][c].begin(), stump_bombs[r][c].end());
                stump_bombs[r][c].erase(unique(stump_bombs[r][c].begin(), stump_bombs[r][c].end()), stump_bombs[r][c].end());
                int s = stump_bombs[r][c].size();
                for (int i = 0; i < s; i++) f1[stump_bombs[r][c][i]]++;

                if (s > 1)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++) f2[{stump_bombs[r][c][i], stump_bombs[r][c][j]}]++;

                if (s > 2)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++)
                            for (int k = j + 1; k < s; k++) f3[{stump_bombs[r][c][i], stump_bombs[r][c][j], stump_bombs[r][c][k]}]++;

                if (s > 3)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++)
                            for (int k = j + 1; k < s; k++)
                                for (int l = k + 1; l < s; l++) f4[{stump_bombs[r][c][i], stump_bombs[r][c][j], stump_bombs[r][c][k], stump_bombs[r][c][l]}]++;
            }

    int locations = 0;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            if (grid[r][c] == '.') {
                int count = 0;

                vector<int> explode;
                if (left[r][c] != -1 && c - left[r][c] <= R && grid[r][left[r][c]] == '*') explode.emplace_back(dsu.sets[indices[r][left[r][c]]]);
                if (right[r][c] < n && right[r][c] - c <= R && grid[r][right[r][c]] == '*') explode.emplace_back(dsu.sets[indices[r][right[r][c]]]);
                if (up[r][c] != -1 && r - up[r][c] <= R && grid[up[r][c]][c] == '*') explode.emplace_back(dsu.sets[indices[up[r][c]][c]]);
                if (down[r][c] < n && down[r][c] - r <= R && grid[down[r][c]][c] == '*') explode.emplace_back(dsu.sets[indices[down[r][c]][c]]);
                sort(explode.begin(), explode.end());
                explode.erase(unique(explode.begin(), explode.end()), explode.end());

                int s = explode.size();
                for (int i = 0; i < s; i++) count += f1[explode[i]];

                if (s > 1)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++) count -= f2[{explode[i], explode[j]}];

                if (s > 2)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++)
                            for (int k = j + 1; k < s; k++) count += f3[{explode[i], explode[j], explode[k]}];

                if (s > 3)
                    for (int i = 0; i < s; i++)
                        for (int j = i + 1; j < s; j++)
                            for (int k = j + 1; k < s; k++)
                                for (int l = k + 1; l < s; l++) count -= f4[{explode[i], explode[j], explode[k], explode[l]}];

                for (int col = max(left[r][c] + 1, c - R); col <= min(right[r][c] - 1, c + R); col++)
                    if (grid[r][col] == 'S') {
                        for (int bomb : stump_bombs[r][col])
                            if (binary_search(explode.begin(), explode.end(), bomb)) goto next1;
                        count++;
                        next1:;
                    }

                for (int row = max(up[r][c] + 1, r - R); row <= min(down[r][c] - 1, r + R); row++)
                    if (grid[row][c] == 'S') {
                        for (int bomb : stump_bombs[row][c])
                            if (binary_search(explode.begin(), explode.end(), bomb)) goto next2;
                        count++;
                        next2:;
                    }

                locations += count == stumps;
            }
    cout << locations;
}
