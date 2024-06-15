#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 998244353;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;
        int index = -1, link = -1;

        TrieNode(int range = 26) : next(range, -1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(string &s, int i) {
        int node = 0;
        for (char c : s) {
            int pos = c == 'T';

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
        }

        if (T[node].index == -1) T[node].index = i;
    }

    void build_links() {
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int p = T[v].link;
            for (int c = 0; c < 2; c++) {
                int u = T[v].next[c];

                if (u != -1) {
                    T[u].link = (p == -1) ? 0 : T[p].next[c];
                    q.emplace(u);
                } else T[v].next[c] = (p == -1) ? 0 : T[p].next[c];
            }
        }
    }

    auto size() {
        return T.size();
    }

    auto & operator[](int i) {
        return T[i];
    }
};

vector<vector<long long>> I(int size) {
    vector<vector<long long>> I(size, vector<long long>(size, 0));
    for (int i = 0; i < size; i++) I[i][i] = 1;

    return I;
}

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

vector<vector<long long>> rref(vector<vector<long long>> &matrix) {
    int n = matrix.size(), m = matrix[0].size();
    auto matrix_inv = I(n);

    int rank = 0;
    for (int c = 0; c < m && rank < n; c++) {
        int pivot = rank;
        for (int i = rank + 1; i < n; i++)
            if (matrix[i][c] > matrix[pivot][c]) pivot = i;

        if (!matrix[pivot][c]) continue;
        swap(matrix[pivot], matrix[rank]);
        swap(matrix_inv[pivot], matrix_inv[rank]);

        auto temp = pow(matrix[rank][c], MODULO - 2, MODULO);
        for (int j = 0; j < m; j++) {
            matrix[rank][j] = mul(matrix[rank][j], temp, MODULO);
            matrix_inv[rank][j] = mul(matrix_inv[rank][j], temp, MODULO);
        }

        for (int i = 0; i < n; i++)
            if (i != rank && matrix[i][c]) {
                temp = matrix[i][c];
                for (int j = 0; j < m; j++) {
                    matrix[i][j] -= mul(temp, matrix[rank][j], MODULO);
                    matrix_inv[i][j] -= mul(temp, matrix_inv[rank][j], MODULO);

                    if (matrix[i][j] < 0) matrix[i][j] += MODULO;
                    if (matrix_inv[i][j] < 0) matrix_inv[i][j] += MODULO;
                }
            }

        rank++;
    }

    return matrix_inv;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Trie trie(1, Trie::NA, 2);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        trie.add(s, i);
    }
    trie.build_links();

    vector<vector<long long>> a(n, vector<long long>(n, 0)), p(trie.size(), vector<long long>(n));
    p[0][0] = 1;
    vector<bool> visited(trie.size(), false);
    visited[0] = true;
    int count = 1;
    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        if (trie[v].index != -1) a[trie[v].index] = p[v];
        else {
            int w = trie[v].next[0] ^ trie[v].next[1];
            for (int u : trie[v].next)
                if (!visited[u]) {
                    if (!visited[u ^ w]) p[u][count++] = 1;
                    else
                        for (int i = 0; i < n; i++) {
                            p[u][i] = mul(2, p[v][i], MODULO) - p[w ^ u][i];
                            if (p[u][i] < 0) p[u][i] += MODULO;
                        }

                    visited[u] = true;
                    q.emplace(u);
                }
        }
    }

    auto a_inv = rref(a);
    for (auto v : a_inv[0]) cout << v << " ";
}
