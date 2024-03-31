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

void insert(string &s, int i, vector<TrieNode> &trie) {
    int node = 0;

    for (char ch : s) {
        int c = ch == 'T';

        if (trie[node].next[c] == -1) {
            trie[node].next[c] = trie.size();
            trie.emplace_back();
        }

        node = trie[node].next[c];
    }

    if (trie[node].index == -1) trie[node].index = i;
}

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

vector<long long> ref(vector<vector<long long>> &a) {
    int n = a.size();
    auto solution = I(n);

    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            if (a[j][i]) {
                swap(a[j], a[i]);
                swap(solution[j], solution[i]);

                auto temp = pow(a[i][i], MODULO - 2, MODULO);
                for (int k = 0; k < n; k++) {
                    a[i][k] = mul(a[i][k], temp, MODULO);
                    solution[i][k] = mul(solution[i][k], temp, MODULO);
                }

                for (int k = 0; k < n; k++)
                    if (i != k) {
                        temp = a[k][i];
                        for (int l = 0; l < n; l++) {
                            a[k][l] -= mul(a[i][l], temp, MODULO);
                            solution[k][l] -= mul(solution[i][l], temp, MODULO);

                            if (a[k][l] < 0) a[k][l] += MODULO;
                            if (solution[k][l] < 0) solution[k][l] += MODULO;
                        }
                    }

                break;
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

        insert(s, i, trie);
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
    for (auto v: solution) cout << v << " ";
}
