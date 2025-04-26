#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
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

        prev = n;
        for (int r = n - 1; ~r; r--)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = down[r][c] = r;
            else down[r][c] = prev;
    }

    for (int r = 0; r < n; r++) {
        int prev = -1;
        for (int c = 0; c < n; c++)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = left[r][c] = c;
            else left[r][c] = prev;

        prev = n;
        for (int c = n - 1; ~c; c--)
            if (grid[r][c] == '*' || grid[r][c] == '#') prev = right[r][c] = c;
            else right[r][c] = prev;
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
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            if (grid[r][c] == 'S') {
                stumps++;
                if (left[r][c] != -1 && c - left[r][c] <= R && grid[r][left[r][c]] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[r][left[r][c]]]);
                if (right[r][c] < n && right[r][c] - c <= R && grid[r][right[r][c]] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[r][right[r][c]]]);
                if (up[r][c] != -1 && r - up[r][c] <= R && grid[up[r][c]][c] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[up[r][c]][c]]);
                if (down[r][c] < n && down[r][c] - r <= R && grid[down[r][c]][c] == '*') stump_bombs[r][c].emplace_back(dsu.sets[indices[down[r][c]][c]]);
                sort(stump_bombs[r][c].begin(), stump_bombs[r][c].end());
                stump_bombs[r][c].erase(unique(stump_bombs[r][c].begin(), stump_bombs[r][c].end()), stump_bombs[r][c].end());
            }

    unordered_map<int, int> f1;
    unordered_map<pair<int, int>, int, Hash> f2;
    unordered_map<array<int, 3>, int, Hash> f3;
    unordered_map<array<int, 4>, int, Hash> f4;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            if (grid[r][c] == 'S' && !stump_bombs[r][c].empty()) {
                int s = stump_bombs[r][c].size();
                for (int i = 0; i < s; i++) {
                    f1[stump_bombs[r][c][i]]++;
                    for (int j = i + 1; j < s; j++) {
                        f2[{stump_bombs[r][c][i], stump_bombs[r][c][j]}]++;
                        for (int k = j + 1; k < s; k++) {
                            f3[{stump_bombs[r][c][i], stump_bombs[r][c][j], stump_bombs[r][c][k]}]++;
                            for (int l = k + 1; l < s; l++)
                                f4[{stump_bombs[r][c][i], stump_bombs[r][c][j], stump_bombs[r][c][k], stump_bombs[r][c][l]}]++;
                        }
                    }
                }
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
                for (int i = 0; i < s; i++) {
                    count += f1[explode[i]];
                    for (int j = i + 1; j < s; j++) {
                        count -= f2[{explode[i], explode[j]}];
                        for (int k = j + 1; k < s; k++) {
                            count += f3[{explode[i], explode[j], explode[k]}];
                            for (int l = k + 1; l < s; l++)
                                count -= f4[{explode[i], explode[j], explode[k], explode[l]}];
                        }
                    }
                }

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
