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
        int node = 0;
        T[node].indices.emplace_back(i);
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
            T[node].indices.emplace_back(i);
        }
    }

    int find(const string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;
            if (T[node].next[pos] == -1) return -1;
            node = T[node].next[pos];
        }
        return node;
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
                    int node = trie.find(out);
                    if (~node) indices = trie.T[node].indices;
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
