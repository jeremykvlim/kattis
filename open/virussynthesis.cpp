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
    vector<deque<int>> adj_list;
    vector<int> s, len, prev, link, half;
    int i, size, curr;

    EerTree(string &str, int n, ascii alpha = LOWER) : s(n + 2, -1), len(n + 2, 0), prev(n + 2, 0), link(n + 2, 0), half(n + 2, 0),
                                                       adj_list(n + 2), a(alpha), i(1), size(2), curr(0) {
        link[0] = 1;
        len[1] = -1;
        build(str);
    }

    int suff_link(int v) {
        while (s[i - 1] != s[i - len[v] - 2]) v = link[v];
        return v;
    }

    void build(string &str) {
        for (char ch : str) {
            int c = ch - a;
            s[i++] = c;
            curr = suff_link(curr);

            auto next = [&](int v) {
                for (int u : adj_list[v])
                    if ((char) u == c) return u >> 8;

                return 0;
            };

            if(!next(curr)) {
                int u = next(suff_link(link[curr]));
                link[size] = u;
                prev[size] = curr;
                len[size] = len[curr] + 2;
                adj_list[curr].emplace_front((size << 8) | c);

                int h = next(suff_link(half[curr]));
                while (2 * len[h] > len[size]) h = link[h];
                half[size++] = h;
            }
            curr = next(curr);
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
                        (et.len[i] & 1) ? dp[et.prev[i]] + 2
                                        : min(et.prev[i] ? dp[et.prev[i]] + 1 : 2, dp[et.half[i]] + 1 + et.len[i] / 2 - et.len[et.half[i]])});

            n = min(n, et.i - et.len[i] + dp[i] - 1);
        }

        cout << n << "\n";
    }
}
