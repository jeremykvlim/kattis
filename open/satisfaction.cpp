#include <bits/stdc++.h>
using namespace std;

string shunting_yard(string infix) {
    auto isop = [&](char c) {
        return c == '|' || c == '&' || c == '~';
    };

    auto precedence = [&](char op) -> int {
        if (op == '|') return 1;
        if (op == '&') return 2;
        if (op == '~') return 3;
    };

    auto l_to_r = [&](char op) {
        return op == '|' || op == '&';
    };

    stack<char> ops;
    string rpn;
    for (char c : infix)
        if (isalpha(c) && isupper(c)) rpn += c;
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

    vector<string> program;
    vector<char> vars;
    vector<bool> seen(26, false);
    string s;
    while (cin >> s) {
        program.emplace_back(s);
        for (char c : s)
            if (isupper(c) && !seen[c - 'A']) {
                vars.emplace_back(c);
                seen[c - 'A'] = true;
            }
    }
    sort(vars.begin(), vars.end());

    int n = vars.size(), m = program.size();
    vector<int> indices(26, -1);
    for (int i = 0; i < n; i++) indices[vars[i] - 'A'] = i;

    vector<string> rpns;
    vector<vector<pair<int, bool>>> order;
    vector<pair<int, bool>> checkpoint;
    for (int i = 0; i < m; i++)
        if (program[i] == "if") {
            checkpoint.emplace_back(rpns.size(), false);
            rpns.emplace_back(shunting_yard(program[i + 1]));
            i += 2;
        } else if (program[i] == "fi") checkpoint.pop_back();
        else if (program[i] == "else") checkpoint.back().second = true;
        else if (program[i] == "checkpoint") order.emplace_back(checkpoint);

    vector<bitset<1 << 20>> table(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 1 << n; j++) table[i][j] = (1 << i) & j;

    vector<bitset<1 << 20>> evals(rpns.size());
    for (int i = 0; i < rpns.size(); i++) {
        stack<bitset<1 << 20>> st;
        for (char c : rpns[i]) {
            if (isupper(c)) st.emplace(table[indices[c - 'A']]);
            else {
                auto bs1 = st.top();
                st.pop();
                if (c == '~') st.emplace(~bs1);
                else {
                    auto bs2 = st.top();
                    st.pop();

                    if (c == '&') st.emplace(bs1 & bs2);
                    else st.emplace(bs1 | bs2);
                }
            }
        }
        evals[i] = st.top();
    }

    for (auto checkpoint : order) {
        bitset<1 << 20> bs;
        bs.set();

        for (auto [i, negate] : checkpoint) {
            if (negate) bs &= ~evals[i];
            else bs &= evals[i];
        }

        cout << ">";
        if (bs.none()) cout << "unreachable\n";
        else {
            for (int i = 0; i < n; i++) {
                if ((table[i] & bs) == bs) cout << vars[i];
                else if ((~table[i] & bs) == bs) cout << (char) tolower(vars[i]);
            }
            cout << "\n";
        }
    }
}
