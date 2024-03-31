#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 998244353;

struct TrieNode {
    vector<int> next;
    int index = -1, link = -1;

    TrieNode() {
        next.resize(2, -1);
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

vector<long long> ref(vector<vector<long long>> &matrix) {
    int r = matrix.size(), c = matrix[0].size();
    auto solution = I(r);

    for (int i = 0; i < r; i++) {
        int pivot = i;
        while (pivot < r && !matrix[pivot][i]) pivot++;

        if (pivot == r) continue;

        swap(matrix[i], matrix[pivot]);
        swap(solution[i], solution[pivot]);
        auto temp = matrix[i][i];
        for (int j = 0; j < c; j++) {
            matrix[i][j] = mul(matrix[i][j], pow(temp, MODULO - 2, MODULO), MODULO);
            solution[i][j] = mul(solution[i][j], pow(temp, MODULO - 2, MODULO), MODULO);
        }

        for (int j = 0; j < r; j++)
            if (j != i) {
                temp = matrix[j][i];
                for (int k = 0; k < c; k++) {
                    matrix[j][k] -= mul(temp, matrix[i][k], MODULO);
                    solution[j][k] -= mul(temp, solution[i][k], MODULO);

                    if (matrix[j][k] < 0) matrix[j][k] += MODULO;
                    if (solution[j][k] < 0) solution[j][k] += MODULO;
                }
            }
    }

    return solution[0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<TrieNode> trie;
    trie.emplace_back();
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int node = 0;
        for (char c : s) {
            int pos = c == 'T';

            if (trie[node].next[pos] == -1) {
                trie[node].next[pos] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[pos];
        }

        if (trie[node].index == -1) trie[node].index = i;
    }

    queue<int> q;
    q.emplace(0);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        int p = trie[v].link;
        for (int c = 0; c < 2; c++) {
            int u = trie[v].next[c];

            if (u != -1) {
                trie[u].link = p == -1 ? 0 : trie[p].next[c];
                q.emplace(u);
            } else trie[v].next[c] = p == -1 ? 0 : trie[p].next[c];
        }
    }

    vector<vector<long long>> a(n, vector<long long>(n, 0)), p(trie.size(), vector<long long>(n));
    vector<bool> visited(trie.size(), false);
    visited[0] = true;
    p[0][0] = 1;
    int count = 1;
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

    auto solution = ref(a);
    for (auto v : solution) cout << v << " ";
}
