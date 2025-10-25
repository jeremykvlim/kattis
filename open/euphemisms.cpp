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

    void add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
        }
        T[node].end = true;
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

                if (u != -1) {
                    T[u].link = (l == -1) ? 0 : T[l].next[c];
                    T[u].end = T[u].end || T[T[u].link].end;
                    q.emplace(u);
                } else T[v].next[c] = (l == -1) ? 0 : T[l].next[c];
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

    int ts;
    cin >> ts;

    for (int t = 1; t <= ts; t++) {
        int n;
        cin >> n;

        Trie trie(1, Trie::NUM, 2);
        while (n--) {
            string w;
            cin >> w;

            trie.add(w);
        }
        trie.aho_corasick();

        string text;
        cin >> text;
        reverse(text.begin(), text.end());

        vector<int> dp(trie.size(), 0), temp(trie.size());
        for (char c : text) {
            int pos = c - trie.a;
            for (int node = 0; node < dp.size(); node++) temp[node] = max(dp[node], trie[trie[node].next[pos]].end ? -1 : dp[trie[node].next[pos]] + 1);
            dp = temp;
        }

        cout << "Case #" << t << ": " << text.size() - dp[0] << "\n";
    }
}
