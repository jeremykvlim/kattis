#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, n;
    cin >> p >> n;

    if (p == 1) {
        vector<vector<int>> adj_list(n);
        for (int _ = 0; _ < n - 1; _++) {
            int a, b;
            cin >> a >> b;

            adj_list[a].emplace_back(b);
            adj_list[b].emplace_back(a);
        }

        vector<int> dist(n), prev(n);
        queue<int> q;
        auto bfs = [&](vector<int> srcs = {0}) {
            fill(dist.begin(), dist.end(), -1);
            fill(prev.begin(), prev.end(), -1);
            for (int s : srcs) {
                if (s == -1) continue;
                dist[s] = 0;
                q.emplace(s);
            }
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (int u : adj_list[v])
                    if (dist[u] == -1) {
                        dist[u] = dist[v] + 1;
                        prev[u] = v;
                        q.emplace(u);
                    }
            }
        };
        bfs();
        int l1 = max_element(dist.begin(), dist.end()) - dist.begin();
        bfs({l1});
        int l2 = max_element(dist.begin(), dist.end()) - dist.begin();

        int l3 = l2;
        for (int i = 0; i < dist[l2] / 2; i++) l3 = prev[l3];
        int l4 = dist[l2] & 1 ? prev[l3] : -1;

        vector<bool> path(n, false);
        for (int v = l2; v != -1; v = prev[v]) path[v] = true;

        bfs({l3, l4});
        vector<int> count(n, 0);
        vector<vector<int>> children(n);
        for (int v = 0; v < n; v++)
            if (~prev[v]) {
                count[prev[v]]++;
                children[prev[v]].emplace_back(v);
            }

        string s;
        if (l1 < prev[l1] && l2 < prev[l2]) s = "0";
        else if (l1 > prev[l1] && l2 > prev[l2]) s = "1";
        else {
            s = "";
            if (l1 > prev[l1]) swap(l1, l2);
        }

        bool first = true;
        vector<int> order;
        stack<int> st1, st2;

        auto unstack_side = [&](auto &st) {
            while (!st.empty() && order.size() < n - 2) {
                int v = st.top();
                st.pop();

                order.emplace_back(v);
                if (!--count[prev[v]] && !path[prev[v]] && !path[prev[prev[v]]]) {
                    v = prev[v];
                    if (v < prev[v]) st1.emplace(v);
                    else st2.emplace(v);
                }
            }
        };

        auto add = [&](int l) {
            for (int c : children[prev[l]])
                if (c != l && order.size() < n - 2) order.emplace_back(c);
        };
        while (order.size() < n - 2) {
            order.emplace_back(l1);

            unstack_side(st1);
            if (order.size() == n - 2) break;

            if (prev[l1] != prev[l2]) add(l1);
            else erase(children[prev[l1]], l1);
            if (order.size() == n - 2) break;

            l1 = prev[l1];
            order.emplace_back(l2);
            if (order.size() == n - 2) break;

            add(l2);
            if (order.size() == n - 2) break;

            l2 = prev[l2];

            if (first) {
                first = false;
                for (int v = 0; v < n; v++)
                    if (!count[v] && !path[v] && !path[prev[v]]) {
                        if (v < prev[v]) st1.emplace(v);
                        else st2.emplace(v);
                    }
            }

            unstack_side(st2);
        }

        if (order.size() == n - 2) {
            vector<bool> seen(n, false);
            for (int l : order) seen[l] = true;
            int l = 1e5;
            for (int i = 0; i < n; i++)
                if (!seen[i]) l = min(l, i);
            order.emplace_back(l);
        }

        cout << s << "\n";
        for (int l : order) cout << l << "\n";
        cout << flush;
    } else {
        cin.ignore();
        string s;
        getline(cin, s);

        int a, b;
        if (s.size() > 1) {
            istringstream iss(s);
            iss >> a >> b;
        } else cin >> a >> b;
        n--;

        auto last = [&]() {
            int a, b;
            cin >> a >> b;
            cout << min(a, b);
            exit(0);
        };

        int l1 = s == "1" ? max(a, b) : min(a, b);
        auto process1 = [&](bool move = true) {
            if (move) {
                cout << l1 << "\n" << flush;
                if (--n == 1) last();
                l1 ^= a ^ b;
                cin >> a >> b;
            }

            while (a == l1 || b == l1) {
                cout << (l1 ^ a ^ b) << "\n" << flush;
                if (--n == 1) last();
                cin >> a >> b;
            }
        };
        process1();

        int l2 = s == "0" ? min(a, b) : max(a, b);
        auto process2 = [&](bool move = true) {
            if (move) {
                cout << l2 << "\n" << flush;
                if (--n == 1) last();
                l2 ^= a ^ b;
                cin >> a >> b;
            }

            while (a == l2 || b == l2) {
                cout << (l2 ^ a ^ b) << "\n" << flush;
                if (--n == 1) last();
                cin >> a >> b;
            }
        };
        process2();

        auto process3 = [&]() {
            while (a != l1 && b != l1) {
                cout << max(a, b) << "\n" << flush;
                if (--n == 1) last();
                cin >> a >> b;
            }
        };

        auto process4 = [&]() {
            while (a != l1 && a != l2 && b != l2) {
                cout << min(a, b) << "\n" << flush;
                if (--n == 1) last();
                cin >> a >> b;
            }
        };

        for (;;) {
            process3();
            process1();
            process4();
            process1(false);
            process4();
            process2();
        }
    }
}
