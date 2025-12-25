#include <bits/stdc++.h>
using namespace std;

struct SuffixAutomaton {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        SYM = 32,
        NA = 0
    };

    struct State {
        int len, link;
        vector<int> next;

        State(int range = 26) : len(0), link(0), next(range, 0) {}
    };

    vector<State> SAM;
    ascii a;
    int last, size;

    SuffixAutomaton(int n, ascii alpha = LOWER, int range = 26) : SAM(2 * n, State(range)), a(alpha), last(0), size(1) {
        SAM[0].link = -1;
    }
    SuffixAutomaton(const string &s, ascii alpha = LOWER, int range = 26) : SAM(2 * s.size(), State(range)), a(alpha), last(0), size(1) {
        SAM[0].link = -1;
        for (char c : s) extend(c);
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

    void add(const string &s) {
        last = 0;
        for (char c : s) extend(c);
    }

    int longest_prefix(const string &s, int start = 0) const {
        int matched = 0;
        for (int node = 0, i = start; i < s.size(); i++) {
            if (!SAM[node].next[s[i] - a]) break;
            node = SAM[node].next[s[i] - a];
            matched++;
        }
        return matched;
    }

    auto & operator[](int i) {
        return SAM[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> songs(m);
    for (auto &s : songs) cin >> s;

    SuffixAutomaton sam((m - 1) * n + 1, SuffixAutomaton::ascii::UPPER, 7);
    for (int i = 1; i < m; i++) sam.add(songs[i]);

    vector<bool> unique(n + 1, true);
    for (int i = 1; i < m; i++) {
        int lcs = 0;
        for (; lcs < n && songs[0][n - 1 - lcs] == songs[i][n - 1 - lcs]; lcs++);
        fill(unique.begin() + 1, unique.begin() + lcs + 1, false);
    }

    vector<int> values(n);
    for (int i = 0; i < n; i++) {
        int len = n - i, l = sam.longest_prefix(songs[0], i);

        if (len > l) values[i] = l;
        else values[i] = unique[len] ? len : n;
    }
    sort(values.begin(), values.end());
    int v = values[(n + 1) / 2 - 1];
    cout << (v == n ? -1 : v + 1);
}
