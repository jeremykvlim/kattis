#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto look = [&](int x, int y) -> pair<int, int> {
        cout << "? " << x + 1 << " " << y + 1 << "\n" << flush;

        int a, b;
        cin >> a >> b;
        return {a - 1, b - 1};
    };

    vector<int> cards(n), indices(n);
    auto assign = [&](int i, int c) {
        cards[i] = c;
        indices[c] = i;
    };

    vector<vector<array<int, 3>>> adj_list(n / 2);
    for (int i = 0; i < n / 2; i++) {
        int x = i << 1, y = i << 1 | 1;

        auto [a, b] = look(x, y);
        if (a == b) {
            assign(x, a);
            assign(y, a);
            continue;
        }
        adj_list[a].push_back({b, x, y});
        adj_list[b].push_back({a, x, y});
    }

    stack<array<int, 4>> st;
    auto dfs = [&](auto &&self, int v) {
        if (adj_list[v].empty()) return;

        auto [c, x1, y1] = adj_list[v][0];
        if (adj_list[v].size() == 1) {
            if (adj_list[c].size() == 1) {
                auto [d, x2, y2] = adj_list[c][0];
                st.push({v, c, x2, y2});
                adj_list[v].clear();
                adj_list[c].clear();
            } else self(self, c);
            return;
        }

        auto [d, x2, y2] = adj_list[v][1];
        adj_list[v].clear();
        if (c == d) {
            adj_list[c].clear();

            int a = -1, b = 0;
            while (a != b) {
                swap(x2, y2);
                tie(a, b) = look(x1, x2);
            }
            assign(x1, a);
            assign(x2, a);
            b = b == v ? c : v;
            assign(y1, b);
            assign(y2, b);
            return;
        }

        auto remove = [&](int i) {
            if (adj_list[i][0][0] == v) adj_list[i].erase(adj_list[i].begin());
            else adj_list[i].erase(adj_list[i].begin() + 1);
        };
        remove(c);
        remove(d);

        auto [a, b] = look(x1, x2);
        if (a == b) {
            assign(x1, a);
            assign(x2, b);
            assign(y1, c);
            assign(y2, d);
            goto done;
        } else if (a != v && b != v) {
            assign(y1, v);
            assign(y2, v);
        } else {
            if (a == v) swap(a, b);
            if (a == c) {
                assign(y1, v);
                assign(y2, d);
            } else {
                assign(y1, c);
                assign(y2, v);
            }
        }
        adj_list[a].push_back({b, x1, x2});
        adj_list[b].push_back({a, x1, x2});
        done:;
        self(self, v);
        self(self, c);
        self(self, d);
    };
    for (int i = 0; i < n / 2; i++) dfs(dfs, i);

    while (st.size() > 1) {
        auto [s, t, x1, y1] = st.top();
        st.pop();
        auto [u, v, x2, y2] = st.top();
        st.pop();

        auto [a, b] = look(x1, x2);
        if (a != s && a != t) swap(a, b);

        assign(y1, a == s ? t : s);
        assign(y2, b == u ? v : u);
        st.push({a, b, x1, x2});
    }

    if (st.size() == 1) {
        auto [c, d, x, y] = st.top();
        int i = indices[c];

        int a = -1, b = 0;
        while (a != b) {
            swap(x, y);
            tie(a, b) = look(i, x);
        }
        cards[x] = cards[i] = c;
        cards[y] = d;
    }

    cout << "! ";
    for (int a : cards) cout << a + 1 << " ";
    cout << "\n" << flush;
}
