#include <bits/stdc++.h>
using namespace std;

struct SuffixAutomaton {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        NA = 0
    };

    struct State {
        int len, link;
        bool end;
        vector<int> next;

        State(int range = 26) : len(0), link(0), end(false), next(range, 0) {}
    };

    vector<State> SAM;
    ascii a;
    int last, size;
    vector<int> occ;
    vector<long long> count;

    SuffixAutomaton(string &s, ascii alpha = LOWER, int range = 26) : SAM(2 * s.size(), State(range)), a(alpha), last(0), size(1),
                                                                      occ(2 * s.size(), 0), count(2 * s.size(), 0) {
        SAM[0].link = -1;
        for (char &c : s) extend(c);
        for (int p = last; p; p = SAM[p].link) SAM[p].end = true;
    }

    void extend(char c) {
        int v = size++;
        SAM[v].len = SAM[last].len + 1;

        int p = last;
        while (p != -1 && !SAM[p].next[c - a]) {
            SAM[p].next[c - a] = v;
            p = SAM[p].link;
        }

        if (p == -1) SAM[v].link = 0;
        else {
            int q = SAM[p].next[c - a];
            if (SAM[p].len + 1 == SAM[q].len) SAM[v].link = q;
            else {
                int clone = size++;
                SAM[clone].len = SAM[p].len + 1;
                SAM[clone].next = SAM[q].next;
                SAM[clone].link = SAM[q].link;
                while (p != -1 && SAM[p].next[c - a] == q) {
                    SAM[p].next[c - a] = clone;
                    p = SAM[p].link;
                }
                SAM[q].link = SAM[v].link = clone;
            }
        }

        last = v;
    }

    void dfs_occurrences(int v = 0) {
        if (occ[v]) return;

        occ[v] = SAM[v].end;
        for (int u : SAM[v].next)
            if (u) {
                dfs_occurrences(u);
                occ[v] += occ[u];
            }
    }

    void dfs_distinct(int v = 0) {
        if (count[v]) return;

        for (int u : SAM[v].next)
            if (u) {
                dfs_distinct(u);
                count[v] += count[u] + 1;
            }
    }

    auto & operator[](int i) {
        return SAM[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int range = 'z' - 'A' + 1;
    SuffixAutomaton sam(s, SuffixAutomaton::ascii::UPPER, range);
    sam.dfs_occurrences();
    sam.dfs_distinct();

    map<char, long long> chars;
    for (int i = 0; i < 2 * s.size(); i++) {
        int l = !i ? 1 : sam[i].len - sam[sam[i].link].len;
        for (int c = 0; c < range; c++)
            if (sam[i].next[c]) chars[c + 'A'] += l * (sam.count[sam[i].next[c]] + 1);

        int temp = sam.occ[i];
        while (i && temp) {
            chars[(temp % 10) + '0'] += l;
            temp /= 10;
        }
    }

    for (auto &[c, occ] : chars) cout << c << " " << occ << "\n";
}
