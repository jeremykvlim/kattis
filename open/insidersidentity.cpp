#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        SYM = 32,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;
        int link;
        bool end;

        TrieNode(int range = 26) : next(range, -1), link(-1), end(false) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(const string &s) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
        }
        T[v].end = true;
    }

    void aho_corasick() {
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int l = T[v].link;
            for (int c = 0; c < r; c++) {
                int u = T[v].next[c];

                if (~u) {
                    T[u].link = !~l ? 0 : T[l].next[c];
                    T[u].end = T[u].end || T[T[u].link].end;
                    q.emplace(u);
                } else T[v].next[c] = !~l ? 0 : T[l].next[c];
            }
        }
        T[0].link = 0;
    }

    int size() {
        return T.size();
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string p;
    cin >> n >> p;

    int m = p.size();
    if (m > n) {
        cout << 0;
        exit(0);
    }

    Trie trie(1, Trie::NUM, 2);
    auto dfs = [&](auto &&self, int i = 0) -> void {
        if (i == m) {
            trie.add(p);
            return;
        }

        if (p[i] == '*') {
            p[i] = '0';
            self(self, i + 1);
            p[i] = '1';
            self(self, i + 1);
            p[i] = '*';
        } else self(self, i + 1);
    };
    dfs(dfs);

    trie.aho_corasick();
    int k = trie.size();
    vector<long long> dp(k, 0), temp(k);
    dp[0] = 1;
    for (int i = 0; i < n; i++) {
        fill(temp.begin(), temp.end(), 0);
        for (int v = 0; v < k; v++)
            if (dp[v])
                for (int c = 0; c < 2; c++) {
                    int u = trie[v].next[c];
                    if (!trie[u].end) temp[u] += dp[v];
                }
        dp = temp;
    }
    cout << (1LL << n) - accumulate(dp.begin(), dp.end(), 0LL);
}