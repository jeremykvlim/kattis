#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

string shunting_yard(const string &infix) {
    auto isop = [&](char c) {
        return c == '^' || c == '|' || c == '&' || c == '=' || c == '!';
    };

    auto precedence = [&](char op) -> int {
        if (op == '^') return 1;
        if (op == '|') return 2;
        if (op == '&') return 3;
        if (op == '=') return 4;
        if (op == '!') return 5;
    };

    auto l_to_r = [&](char op) {
        return op == '|' || op == '&';
    };

    stack<char> ops;
    string rpn;
    for (char c : infix)
        if (c == ' ') continue;
        else if (c == 'x' || c == 'y' || c == '0' || c == '1') rpn += c;
        else if (c == '(') ops.emplace(c);
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                rpn += ops.top();
                ops.pop();
            }
            if (!ops.empty() && ops.top() == '(') ops.pop();
        } else if (isop(c)) {
            while (!ops.empty() && isop(ops.top())) {
                char op = ops.top();
                if ((l_to_r(c) && precedence(c) <= precedence(op)) || (!l_to_r(c) && precedence(c) < precedence(op))) {
                    rpn += op;
                    ops.pop();
                } else break;
            }
            ops.emplace(c);
        }

    while (!ops.empty()) {
        rpn += ops.top();
        ops.pop();
    }

    return rpn;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<array<int, 5>> valid;
    vector<vector<vector<bool>>> visited(k, vector<vector<bool>>(k, vector<bool>(4, false)));
    for (int i = 0; i < n; i++) {
        int a, b, r;
        string expr;
        cin >> a >> b >> expr >> r;
        a--;
        b--;

        auto rpn = shunting_yard(expr);

        int mask = 0;
        for (int bx = 0; bx < 2; bx++)
            for (int by = 0; by < 2; by++) {
                stack<int> st;
                for (char c : rpn) {
                    if (c == 'x') st.emplace(bx);
                    else if (c == 'y') st.emplace(by);
                    else if (c == '0') st.emplace(0);
                    else if (c == '1') st.emplace(1);
                    else if (c == '!') {
                        int a = st.top();
                        st.pop();

                        st.emplace(a ^ 1);
                    } else {
                        int b = st.top();
                        st.pop();
                        int a = st.top();
                        st.pop();

                        int r = 0;
                        switch (c) {
                            case '=': {
                                r = a == b;
                                break;
                            }
                            case '&': {
                                r = a & b;
                                break;
                            }
                            case '|': {
                                r = a | b;
                                break;
                            }
                            case '^': {
                                r = a ^ b;
                                break;
                            }
                        }
                        st.emplace(r);
                    }
                }
                if (st.top()) mask |= 1 << (2 * bx + by);
            }
        if (!mask) continue;

        for (int bx = 0; bx < 2; bx++)
            for (int by = 0; by < 2; by++) {
                int shift = 2 * bx + by;
                if (!((mask >> shift) & 1)) continue;
                if (!visited[a][b][shift]) {
                    visited[a][b][shift] = true;
                    valid.push_back({a, b, bx, by, r});
                }
            }
    }

    int returned;
    cin >> returned;

    int m = 1 << k;
    vector<dynamic_bitset<>> f(m, dynamic_bitset<>(m)), y_used(m, dynamic_bitset<>(m));
    for (int x = 0; x < m; x++) {
        if (returned == 1) f[x].set();
        
        int remaining = m;
        for (auto [a, b, bx, by, r] : valid) {
            if (((x >> (k - 1 - a)) & 1) != bx) continue;

            dynamic_bitset<> temp(m);
            for (int y = 0; y < m; y++) {
                if (y_used[x][y] || ((y >> (k - 1 - b)) & 1) != by) continue;
                temp[y] = true;
            }
            if (temp.none()) continue;

            if (r == 1) f[x] |= temp;
            else f[x] &= ~temp;

            y_used[x] |= temp;
            remaining -= temp.count();
            if (!remaining) break;
        }
    }

    int reflex = 0, symm = 0, trans = 0;
    for (int x = 0; x < m; x++)
        if (f[x][x]) reflex++;

    for (int x = 0; x < m; x++)
        for (int y = 0; y < m; y++)
            if (f[x][y] && f[y][x]) symm++;

    vector<dynamic_bitset<>> g(m, dynamic_bitset<>(m));
    for (int z = 0; z < m; z++)
        for (int x = 0; x < m; x++)
            if (f[x][z]) g[z][x] = true;

    for (int x = 0; x < m; x++)
        for (int z = 0; z < m; z++)
            if (!f[x][z]) trans += ((f[x] & g[z]).count());

    cout << reflex << " " << symm << " " << trans;
}
