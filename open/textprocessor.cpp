#include <bits/stdc++.h>
using namespace std;

struct SuffixTree {
    struct Trie {
        enum ascii {
            LOWER = 97,
            UPPER = 65,
            NUM = 48,
            NA = 0
        };

        struct TrieNode {
            vector<int> next;

            TrieNode(int range = 26) : next(range, -1) {}
        };

        vector<TrieNode> T;
        ascii a;
        int r;

        Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

        auto & operator[](int i) {
            return T[i];
        }
    };
    
    string s;
    Trie trie;
    vector<int> l, r, parent, link;
    int node = 0, pos = 0, size = 2, index = 0, leaves = 0;
    long long count = 0;
    queue<int> order;

    void add(int c) {
        if (pos >= r[node]) {
            if (trie[node].next[c] == -1) {
                trie[node].next[c] = size;
                leaves++;
                order.emplace(size);

                l[size] = index;
                parent[size++] = node;
                node = link[node];
                pos = r[node];

                add(c);
                return;
            }

            node = trie[node].next[c];
            pos = l[node];
        }

        if (pos == -1 || c == s[pos] - 'a') pos++;
        else {
            l[size + 1] = index;
            l[size] = l[node];
            l[node] = r[size] = pos;

            parent[size + 1] = size;
            parent[size] = parent[node];
            parent[node] = size;

            trie[parent[size]].next[s[l[size]] - 'a'] = size;
            trie[size].next[s[pos] - 'a'] = node;
            trie[size].next[c] = size + 1;
            leaves++;
            order.emplace(size + 1);

            node = link[parent[size]];
            pos = l[size];
            while (pos < r[size]) {
                node = trie[node].next[s[pos] - 'a'];
                pos += r[node] - l[node];
            }
            link[size] = (pos == r[size]) ? node : size + 2;
            pos = r[node] + r[size] - pos;
            size += 2;

            add(c);
            return;
        }

        count += leaves;
        index++;
    }

    void remove(int i) {
        int v = order.front();
        order.pop();
        if (v == node) {
            int len = i - pos;
            count -= len;
            l[v] += len;
            pos = l[v];

            node = link[parent[v]];
            node = trie[node].next[s[pos] - 'a'];
            while (pos + r[node] - l[node] < i) {
                pos += r[node] - l[node];
                node = trie[node].next[s[pos] - 'a'];
            }
            pos = l[node] + i - pos;
            order.emplace(v);
        } else {
            leaves--;
            count -= i - l[v];

            int p = parent[v];
            trie[p].next[s[l[v]] - 'a'] = -1;

            if (p) {
                int ch = -1;
                for (int c : trie[p].next)
                    if (c != -1) {
                        if (ch != -1) return;
                        else ch = c;
                    }

                trie[parent[p]].next[s[l[p]] - 'a'] = ch;
                parent[ch] = parent[p];
                l[ch] -= r[p] - l[p];
                if (node == p) {
                    node = ch;
                    pos += l[ch] - l[p];
                }
            }
        }
    }

    SuffixTree(string &s) : s(s),
                            trie(2 * s.size() + 1),
                            l(2 * s.size() + 1, 0),
                            r(2 * s.size() + 1, s.size()),
                            link(2 * s.size() + 1, 0),
                            parent(2 * s.size() + 1, 0) {
        fill(trie[1].next.begin(), trie[1].next.end(), 0);
        link[0] = 1;
        l[0] = l[1] = -1;
        r[0] = r[1] = parent[0] = parent[1] = 0;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q, w;
    cin >> s >> q >> w;

    SuffixTree st(s);
    for (int i = 0; i < w; i++) st.add(s[i] - 'a');

    vector<long long> count(s.size() + 1, st.count);
    for (int i = w; i < s.size(); i++) {
        st.remove(i);
        st.add(s[i] - 'a');
        count[i - w + 2] = st.count;
    }

    while (q--) {
        int i;
        cin >> i;
        cout << count[i] << "\n";
    }
}
