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
        bool end;
        vector<int> next;

        State(int range = 26) : len(0), link(0), end(false), next(range, 0) {}
    };

    vector<State> SAM;
    ascii a;
    int last, size;

    SuffixAutomaton(string s, ascii alpha = LOWER, int range = 26) : SAM(2 * s.size(), State(range)), a(alpha), last(0), size(1) {
        SAM[0].link = -1;
        for (char c : s) extend(c);
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

    auto & operator[](int i) {
        return SAM[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool print = false;
    for (int k, n = 1; cin >> k && k; n++) {
        cin.ignore();
        if (print) cout << "\n";
        print = true;

        string s;
        getline(cin, s);

        string tdp;
        while (getline(cin, s)) {
            if (s == "END TDP CODEBASE") break;
            tdp += s + "\n";
        }

        getline(cin, s);

        string jcn;
        while (getline(cin, s)) {
            if (s == "END JCN CODEBASE") break;
            jcn += s + "\n";
        }

        int m = jcn.size();
        SuffixAutomaton sam(tdp, SuffixAutomaton::NA, 128);

        vector<int> dp1(m, 0);
        for (int i = 0, l = 0, state = 0; i < m; i++) {
            int c = (char) jcn[i];
            if (sam[state].next[c]) {
                l++;
                state = sam[state].next[c];
            } else {
                while (state != -1 && !sam[state].next[c]) state = sam[state].link;
                if (state == -1) l = state = 0;
                else {
                    l = min(l, sam[state].len) + 1;
                    state = sam[state].next[c];
                }
            }
            dp1[i] = l;
        }

        vector<int> dp2(m, 0);
        for (int i = 0, j = 0; i < m; i++) {
            if (i) {
                int l = max(dp2[i - 1] - 1, 0);
                if (j < i + l) j = i + l - 1;
            }
            while (j < m && dp1[j] > j - i) j++;
            dp2[i] = j - i;
        }

        vector<pair<int, int>> segments;
        for (int i = 0, r = -1; i < m; i++)
            if (r < i + dp2[i] && dp2[i]) {
                r = i + dp2[i];
                segments.emplace_back(dp2[i], i);
            }
        sort(segments.begin(), segments.end(), [](const auto &p1, const auto &p2) {return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second;});

        cout << "CASE " << n << "\n";
        for (int i = 0; i < segments.size() && i < k; i++) {
            auto [l, p] = segments[i];
            cout << "INFRINGING SEGMENT " << i + 1 << " LENGTH " << l << " POSITION " << p << "\n" << jcn.substr(p, l) << "\n";
        }
    }
}
