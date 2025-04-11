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
        int count;
        bool end;

        TrieNode(int range = 26) : next(range, -1), count(0), end(false) {}
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
            T[node].count++;
        }
        T[node].end = true;
    }

    int find(string &s) {
        int node = 0;
        for (char c : s) node = T[node].next[c - a];
        return node;
    }

    string order_statistic(int k, int node = 0) {
        string word;
        for (;;) {
            if (T[node].end)
                if (!--k) return word;

            for (int c = 0; c < r; c++)
                if (T[node].next[c] != -1) {
                    if (T[T[node].next[c]].count >= k) {
                        word += c + a;
                        node = T[node].next[c];
                        break;
                    } else k -= T[T[node].next[c]].count;
                }
        }
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Trie trie;
    while (n--) {
        string word;
        cin >> word;

        trie.add(word);
    }

    int q;
    cin >> q;

    while (q--) {
        string s;
        cin >> s;

        string text;
        int node = 0;
        for (int i = 0; i < s.size()
            if (s[i] == '#') {
                int tab = 0;
                for (; i < s.size() && s[i] == '#'; i++, tab++);
                if (node != -1) {
                    int count = trie[node].count - trie[node].end;
                    if (count <= 0) continue;

                    text += trie.order_statistic((tab - 1) % count + 1 + trie[node].end, node);
                    node = trie.find(text);
                }
            } else {
                char c = s[i++];
                text += c;
                if (node != -1) node = trie[node].next[c - 'a'];
            }
        cout << text << "\n";
    }
}
