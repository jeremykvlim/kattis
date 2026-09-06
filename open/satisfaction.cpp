#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

struct ReducedOrderedBinaryDecisionDiagram {
    enum Operation {
        NOR = 0b0001,
        NIMPLY = 0b0100,
        XOR = 0b0110,
        NAND = 0b0111,
        AND = 0b1000,
        XNOR = 0b1001,
        IMPLY = 0b1011,
        OR = 0b1110
    };

    int n;
    vector<array<int, 3>> nodes;
    unordered_map<array<int, 3>, int, Hash> node_indices;
    vector<int> neg_memo;
    array<unordered_map<pair<int, int>, int, Hash>, 15> memo;

    ReducedOrderedBinaryDecisionDiagram(int n) : n(n) {
        nodes.push_back({n, 0, 0});
        nodes.push_back({n, 1, 1});
        neg_memo.emplace_back(1);
        neg_memo.emplace_back(0);
        for (int i = 0; i < n; i++) node(i);
    }

    int node(int x, int low = 0, int high = 1) {
        if (low == high) return low;

        auto it = node_indices.find({x, low, high});
        if (it != node_indices.end()) return it->second;

        neg_memo.emplace_back(-1);
        nodes.push_back({x, low, high});
        return node_indices[{x, low, high}] = nodes.size() - 1;
    }

    int apply(int a, int b, Operation op) {
        if (!(((op >> 1) ^ (op >> 2)) & 1) && a > b) swap(a, b);
        if (a <= 1) {
            int u = (op >> (a << 1)) & 3;
            if (!u) return 0;
            if (u == 1) return neg(b);
            if (u == 2) return b;
            return 1;
        }

        if (b <= 1) {
            int u = ((op >> b) & 1) | ((op >> (b + 1)) & 2);
            if (!u) return 0;
            if (u == 1) return neg(a);
            if (u == 2) return a;
            return 1;
        }

        if (a == b) {
            int u = (op & 1) | ((op >> 2) & 2);
            if (!u) return 0;
            if (u == 1) return neg(a);
            if (u == 2) return a;
            return 1;
        }

        auto &m = memo[op];
        auto it = m.find({a, b});
        if (it != m.end()) return it->second;

        int xa = nodes[a][0], xb = nodes[b][0], x = min(xa, xb);
        return m[{a, b}] = node(x,
                                apply(xa == x ? nodes[a][1] : a, xb == x ? nodes[b][1] : b, op),
                                apply(xa == x ? nodes[a][2] : a, xb == x ? nodes[b][2] : b, op));
    }

    int neg(int a) {
        if (a <= 1) return a ^ 1;
        if (~neg_memo[a]) return neg_memo[a];

        auto [x, low, high] = nodes[a];
        int b = node(x, neg(low), neg(high));
        neg_memo[b] = a;
        return neg_memo[a] = b;
    }

    int conj(int a, int b) {
        return apply(a, b, AND);
    }

    int disj(int a, int b) {
        return apply(a, b, OR);
    }

    int excl_disj(int a, int b) {
        return apply(a, b, XOR);
    }

    int impl(int a, int b) {
        return apply(a, b, IMPLY);
    }

    int non_conj(int a, int b) {
        return apply(a, b, NAND);
    }

    int non_disj(int a, int b) {
        return apply(a, b, NOR);
    }

    int excl_non_disj(int a, int b) {
        return apply(a, b, XNOR);
    }

    int non_impl(int a, int b) {
        return apply(a, b, NIMPLY);
    }

    tuple<bool, int, int> query(int a, int y = 0) {
        if (!a) return {false, 0, 0};
        if (a == 1) return {true, (1 << n) - (1 << y), 0};

        auto [x, low, high] = nodes[a];
        auto [low_SAT, low_OR_mask, low_AND_mask] = query(low, x + 1);
        auto [high_SAT, high_OR_mask, high_AND_mask] = query(high, x + 1);
        if (!low_SAT && !high_SAT) return {false, 0, 0};

        int OR_mask = (low_SAT ? low_OR_mask : 0) | (high_SAT ? high_OR_mask | (1 << x) : 0),
            AND_mask = (low_SAT ? low_AND_mask : ((1 << n) - 1)) & (high_SAT ? high_AND_mask | (1 << x) : ((1 << n) - 1)),
            extra = (1 << x) - (1 << y);
        return {true, OR_mask | extra, AND_mask & ~extra};
    }
};

string shunting_yard(const string &infix) {
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
    vector<bool> seen(26, false);
    string s;
    while (cin >> s) {
        program.emplace_back(s);
        for (char c : s)
            if (isupper(c)) seen[c - 'A'] = true;
    }

    string vars;
    vector<int> indices(26, -1);
    for (int i = 0; i < 26; i++)
        if (seen[i]) {
            vars += (char) (i + 'A');
            indices[i] = vars.size() - 1;
        }

    ReducedOrderedBinaryDecisionDiagram robdd(vars.size());
    unordered_map<string, int> expressions;
    unordered_map<int, string> outputs;
    stack<pair<int, int>> branches;
    for (int i = 0, a = 1; i < program.size(); i++)
        if (program[i] == "if") {
            auto [it, inserted] = expressions.try_emplace(program[i + 1]);
            if (inserted) {
                stack<int> st;
                for (char c : shunting_yard(program[i + 1]))
                    if (isupper(c)) st.emplace(indices[c - 'A'] + 2);
                    else if (c == '~') st.top() = robdd.neg(st.top());
                    else {
                        int b = st.top();
                        st.pop();
                        st.top() = c == '&' ? robdd.conj(st.top(), b) : robdd.disj(st.top(), b);
                    }
                it->second = st.top();
            }
            int b = it->second;
            branches.emplace(a, b);
            a = robdd.conj(a, b);
            i += 2;
        } else if (program[i] == "else") {
            auto [p, b] = branches.top();
            a = robdd.conj(p, robdd.neg(b));
        } else if (program[i] == "fi") {
            a = branches.top().first;
            branches.pop();
        } else if (program[i] == "checkpoint") {
            auto [it, inserted] = outputs.try_emplace(a, ">");
            if (inserted) {
                auto [SAT, OR_mask, AND_mask] = robdd.query(a);
                if (!SAT) it->second += "unreachable";
                else
                    for (int j = 0; j < vars.size(); j++)
                        if ((AND_mask >> j) & 1) it->second += vars[j];
                        else if (!((OR_mask >> j) & 1)) it->second += tolower(vars[j]);
            }
            cout << it->second << "\n";
        }
}
