#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    void reset() {
        fill(BIT.begin(), BIT.end(), 0);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, C;
    cin >> r >> C;
    cin.ignore();

    int c = (C + 1) / 2;
    vector<int> vertices(r, c);
    if (C & 1)
        for (int row = 1; row < r; row += 2) vertices[row]--;

    auto read = [&] {
        string line;
        getline(cin, line);

        line.insert(line.begin(), '#');
        line.resize(2 * C + 1, '#');
        return line;
    };

    vector<vector<int>> left_edge(r, vector<int>(c, 0)), right_edge(r, vector<int>(c, 0)), straight_edge(r, vector<int>(c, 0));
    for (int row = 0; row < r; row++) {
        auto line = read();

        int v = vertices[row], offset = 2 * (row & 1);
        for (int k = v - 2; k >= 0; k--)
            if (line[offset + 4 * k + 2] == '-') straight_edge[row][k] = straight_edge[row][k + 1] + 1;
        if (row == r - 1) break;

        line = read();
        for (int k = 0; k < v; k++) {
            left_edge[row][k] = line[offset + 4 * k] == '/';
            right_edge[row][k] = line[offset + 4 * k + 2] == '\\';
        }
    }

    for (int row = r - 2; ~row; row--) {
        int v = vertices[row], shift = row & 1;
        for (int k = 0; k < v; k++) {
            if (left_edge[row][k]) left_edge[row][k] += left_edge[row + 1][k + shift - 1];
            if (right_edge[row][k]) right_edge[row][k] += right_edge[row + 1][k + shift];
        }
    }

    int n = max(r, c);
    FenwickTree<int> fw(n + 1);
    vector<vector<int>> indices(n);
    vector<int> len1(n, 0), len2(n, 0);
    auto sweep = [&](int v) {
        fw.reset();
        for (int i = 0; i < v; i++) indices[i].clear();
        for (int k = 0; k < v; k++) indices[max(0, k - len1[k])].emplace_back(k);

        int sum = 0;
        for (int i = 0; i < v; i++) {
            for (int k : indices[i]) fw.update(k + 1, 1);
            sum += fw.range_sum_query(i + 1, i + len2[i] + 1);
        }
        return sum;
    };

    auto triangles = 0LL;
    for (int row = 0; row < r; row++) {
        int v = vertices[row];
        for (int k = 0; k < v; k++) {
            len1[k] = left_edge[row][k];
            len2[k] = min(straight_edge[row][k], right_edge[row][k]);
        }
        triangles += sweep(v);
    }

    for (int i = 0; i < (r + C + 1) / 2; i++) {
        int v = 0;
        for (int row = 0; row < r; row++) {
            int k = i - (row + 1) / 2;
            if (0 <= k && k < vertices[row]) {
                len1[v] = straight_edge[row][k];
                len2[v] = min(left_edge[row][k], right_edge[row][k]);
                v++;
            }
        }
        triangles += sweep(v);
    }
    cout << triangles;
}
