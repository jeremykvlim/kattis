#include <bits/stdc++.h>
using namespace std;

struct EerTree {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        NA = 0
    };

    ascii a;
    vector<deque<int>> next;
    vector<int> s, len, parent, link, half;
    int i, size, last;

    EerTree(string &str, int n, ascii alpha = LOWER) : s(n + 2, -1), len(n + 2, 0), parent(n + 2, 0), link(n + 2, 0), half(n + 2, 0),
                                                       next(n + 2), a(alpha), i(1), size(2), last(0) {
        link[0] = 1;
        len[1] = -1;
        build(str);
    }

    int suff_link(int v) {
        while (s[i - 1] != s[i - len[v] - 2]) v = link[v];
        return v;
    }

    int get(int v, int c) const {
        for (int ch : next[v])
            if ((char) ch == c) return ch >> 8;

        return 0;
    }

    void build(string &str) {
        for (char ch : str) {
            int c = ch - a;
            s[i++] = c;
            last = suff_link(last);
            if(!get(last, c)) {
                int u = get(suff_link(link[last]), c);
                link[size] = u;
                parent[size] = last;
                len[size] = len[last] + 2;
                next[last].emplace_front((size << 8) | c);

                int h = get(suff_link(half[last]), c);
                while (2 * len[h] > len[size]) h = link[h];
                half[size++] = h;
            }
            last = get(last, c);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<int> dp;
    while (t--) {
        string s;
        cin >> s;

        int n = s.size();
        EerTree et(s, n, EerTree::ascii::UPPER);

        dp.resize(et.size, 1e9);
        for (int i = 2; i < et.size; i++) {
            dp[i] = min({et.len[i], dp[et.link[i]] + et.len[i] - et.len[et.link[i]],
                        (et.len[i] & 1) ? dp[et.parent[i]] + 2
                                        : min(et.parent[i] ? dp[et.parent[i]] + 1 : 2, dp[et.half[i]] + 1 + et.len[i] / 2 - et.len[et.half[i]])});

            n = min(n, et.i - et.len[i] + dp[i] - 1);
        }

        cout << n << "\n";
    }
}
