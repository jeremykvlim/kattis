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
        int link, hash;

        TrieNode(int range = 26) : next(range, -1), link(-1), hash(0) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(const string &s, int h) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
        }
        T[v].hash += h;
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
                    T[u].hash += T[T[u].link].hash;
                    q.emplace(u);
                } else T[v].next[c] = !~l ? 0 : T[l].next[c];
            }
        }
        T[0].link = 0;
    }

    bool balanced(const string &s) {
        int v = 0, sum = 0;
        for (char c : s) {
            int pos = c - a;

            for (; !~T[v].next[pos]; v = T[v].link);
            v = T[v].next[pos];
            sum += T[v].hash;
        }
        return !sum;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    cin.ignore();

    Trie trie(1, Trie::ascii::LOWER, 27);
    vector<string> p(n);
    for (auto &pi : p) {
        getline(cin, pi);

        replace(pi.begin(), pi.end(), ' ', '{');
    }

    mt19937 rng(random_device{}());
    vector<int> hash(n);
    while (m--) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;

        int h = rng();
        hash[u] += h;
        hash[v] -= h;
    }
    cin.ignore();

    for (int i = 0; i < n; i++) trie.add(p[i], hash[i]);
    trie.aho_corasick();

    while (k--) {
        string ti;
        getline(cin, ti);

        replace(ti.begin(), ti.end(), ' ', '{');
        cout << (trie.balanced(ti) ? "yes\n" : "no\n");
    }
}
