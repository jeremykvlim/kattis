#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM   = 48,
        SYM   = 32,
        NA    = 0
    };

    struct TrieNode {
        vector<int> next, indices;

        TrieNode(int range = 26) : next(range, -1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(const string &s, int i) {
        int v = 0;
        T[v].indices.emplace_back(i);
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
            T[v].indices.emplace_back(i);
        }
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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    Trie trie(1, Trie::ascii::SYM, 95);
    vector<string> history;
    while (n--) {
        string in;
        getline(cin, in);

        bool up = false;
        int i = -1, count = 0;
        vector<int> indices;
        string out;
        for (char c : in)
            if (c == '^') {
                if (!up) {
                    up = true;
                    count = 0;
                    int v = trie.find(out);
                    if (~v) indices = trie.T[v].indices;
                    else indices.clear();
                }

                if (!indices.empty()) {
                    count = min(count + 1, (int) indices.size());
                    i = indices[indices.size() - count];
                } else {
                    i = -1;
                    count = 0;
                }
            } else {
                if (up) {
                    if (~i) out = history[i];
                    up = false;
                }
                out += c;
            }

        if (up && ~i) out = history[i];
        cout << out << "\n";

        history.emplace_back(out);
        trie.add(out, history.size() - 1);
    }
}
