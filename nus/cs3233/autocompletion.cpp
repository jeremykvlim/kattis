#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
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

    void add(const string &s) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
            T[v].count++;
        }
        T[v].end = true;
    }

    int find(const string &s) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;
            if (!~T[v].next[pos]) return -1;
            v = T[v].next[pos];
        }
        return v;
    }

    string kth(int k, int v = 0) {
        string word;
        for (;;) {
            if (T[v].end)
                if (!--k) return word;

            for (int c = 0; c < 26; c++)
                if (~T[v].next[c]) {
                    if (T[T[v].next[c]].count >= k) {
                        word += 'a' + c;
                        v = T[v].next[c];
                        break;
                    } else k -= T[T[v].next[c]].count;
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
        int v = 0;
        for (int i = 0; i < s.size();)
            if (s[i] == '#') {
                int tab = 0;
                for (; i < s.size() && s[i] == '#'; i++, tab++);
                if (~v) {
                    int count = trie[v].count - trie[v].end;
                    if (count <= 0) continue;

                    text += trie.kth((tab - 1) % count + 1 + trie[v].end, v);
                    v = trie.find(text);
                }
            } else {
                char c = s[i++];
                text += c;
                if (~v) v = trie[v].next[c - 'a'];
            }
        cout << text << "\n";
    }
}
