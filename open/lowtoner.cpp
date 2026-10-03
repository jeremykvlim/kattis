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
        bool end;

        TrieNode(int range = 26) : next(range, -1), end(false) {}
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

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<char, vector<char>> prints{{'C', {'C', 'G', 'O', 'Q'}},
                                             {'O', {'O', 'Q'}},
                                             {'F', {'B', 'E', 'F', 'P', 'R'}},
                                             {'P', {'B', 'P', 'R'}},
                                             {'E', {'B', 'E'}},
                                             {'I', {'I', 'T'}},
                                             {'V', {'M', 'V'}}};

    int n;
    cin >> n;

    Trie trie(1, Trie::UPPER);
    while (n--) {
        string word;
        cin >> word;

        trie.add(word);
    }
    cin.ignore();

    string s;
    while (getline(cin, s)) {
        vector<pair<int, string>> seqs{{0, {}}};
        for (char c : s) {
            vector<pair<int, string>> temp;

            for (auto [v, word] : seqs) {
                auto add = [&](int u, char C) {
                    temp.emplace_back(u, word + C);
                };

                if (c == ' ') {
                    if (trie[v].end) add(0, c);

                    for (int C = 0; C < 26; C++)
                        if (~trie[v].next[C]) add(trie[v].next[C], (char) (C + 'A'));
                } else {
                    if (!prints.count(c)) {
                        if (~trie[v].next[c - 'A']) add(trie[v].next[c - 'A'], c);
                        continue;
                    }

                    for (char C : prints[c])
                        if (~trie[v].next[C - 'A']) add(trie[v].next[C - 'A'], C);
                }
            }
            seqs = temp;
        }

        for (auto [v, word] : seqs)
            if (trie[v].end) cout << word << "\n";
        cout << "\n";
    }
}
