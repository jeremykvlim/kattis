#include <bits/stdc++.h>
using namespace std;

struct AbstractSyntaxTree {
    struct ASTNode {
        int l, r;
        char op;
        string s;

        ASTNode(const string &s, char op) : l(-1), r(-1), op(op), s(s) {}
        ASTNode(int l, char op) : l(l), r(-1), op(op), s("") {}
        ASTNode(int l, int r, char op) : l(l), r(r), op(op), s("") {}
    };

    int root;
    vector<ASTNode> AST;

    AbstractSyntaxTree(const vector<string> &rpn) {
        stack<int> st;
        for (auto c : rpn) {
            if (c[0] == '.' || isalnum((unsigned char) c[0])) AST.emplace_back(c, '.');
            else if (c == "_") {
                int i = st.top();
                st.pop();

                AST.emplace_back(i, '_');
            } else if (c == "[") {
                int i = st.top();
                st.pop();

                AST.emplace_back(i, '[');
            } else {
                int r = st.top();
                st.pop();
                int l = st.top();
                st.pop();

                AST.emplace_back(l, r, c[0]);
            }

            st.emplace(AST.size() - 1);
        }

        root = st.top();
    }

    vector<int> post_order_traversal() {
        vector<int> order;
        stack<int> s1, s2;
        s1.emplace(root);
        while (!s1.empty()) {
            int i = s1.top();
            s1.pop();

            s2.emplace(i);

            const auto &node = AST[i];
            if (node.op == '_' || node.op == '[') s1.emplace(node.l);
            else if (node.op != '.') {
                s1.emplace(node.l);
                s1.emplace(node.r);
            }
        }

        while (!s2.empty()) {
            order.emplace_back(s2.top());
            s2.pop();
        }

        return order;
    }

    auto & operator[](int i) {
        return AST[i];
    }

    auto size() {
        return AST.size();
    }
};

vector<string> shunting_yard(const vector<string> &infix) {
    auto isop = [&](const string &c) {
        return c == "=" || c == "+" || c == "-" || c == "*" || c == "/" || c == "^" || c == "_";
    };

    auto precedence = [&](const string &op) -> int {
        if (op == "=") return 1;
        if (op == "+" || op == "-") return 2;
        if (op == "*" || op == "/") return 3;
        if (op == "^") return 4;
        if (op == "_") return 5;
    };

    auto l_to_r = [&](const string &op) {
        return op == "=" || op == "+" || op == "-" || op == "*" || op == "/";
    };

    stack<string> ops;
    vector<string> rpn;
    for (auto &c : infix) {
        if (c[0] == '.' || isalnum((unsigned char) c[0])) rpn.emplace_back(c);
        else if (c == "(" || c == "{") ops.emplace(c);
        else if (c == ")" || c == "}") {
            auto r = c == ")" ? "(" : "{";
            while (!ops.empty() && ops.top() != r) {
                rpn.emplace_back(ops.top());
                ops.pop();
            }
            if (!ops.empty() && ops.top() == r) ops.pop();
            if (c == ")") rpn.emplace_back("[");
        } else {
            while (!ops.empty() && isop(ops.top())) {
                auto op = ops.top();
                if ((l_to_r(c) && precedence(c) <= precedence(op)) || (!l_to_r(c) && precedence(c) < precedence(op))) {
                    rpn.emplace_back(ops.top());
                    ops.pop();
                } else break;
            }
            ops.emplace(c);
        }
    }

    while (!ops.empty()) {
        rpn.emplace_back(ops.top());
        ops.pop();
    }
    return rpn;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool print = false;
    string s;
    while (getline(cin, s)) {
        if (print) cout << "\n";
        print = true;

        vector<string> infix;
        bool in = true;
        for (int i = 0; i < s.size();) {
            char c = s[i];
            if (isdigit(c) || c == '.') {
                int j = i;
                while (i < s.size() && (isdigit(s[i]) || s[i] == '.')) i++;
                infix.emplace_back(s.substr(j, i - j));
                in = false;
            } else if (isalpha(c) || c == ')' || c == '}') {
                infix.emplace_back(string{c});
                i++;
                in = false;
            } else if (c == '-' && in) {
                infix.emplace_back("_");
                i++;
                in = true;
            } else {
                infix.emplace_back(string{c});
                i++;
                in = true;
            }
        }
        AbstractSyntaxTree ast(shunting_yard(infix));
        auto order = ast.post_order_traversal();

        struct Box {
            vector<string> lines;
            int lvc, height, width;

            Box() {}
            Box(int lvc, int h, int w) : lvc(lvc), height(h), width(w), lines(h, string(w, ' ')) {}
            Box(const string &s) : lvc(0), height(1), width(s.size()), lines({s}) {}

            void copy(const Box &b, int r_offset, int c_offset) {
                for (int i = 0; i < b.height; i++)
                    for (int j = 0; j < b.width; j++) lines[i + r_offset][j + c_offset] = b.lines[i][j];
            }
        };

        vector<Box> boxes(ast.size());
        for (int i : order) {
            const auto &node = ast[i];

            if (node.op == '.') boxes[i] = Box(node.s);
            else if (node.op == '_') {
                Box l = boxes[node.l], b(l.lvc, l.height, l.width + 1);
                b.copy(l, 0, 1);
                b.lines[b.lvc][0] = '-';
                boxes[i] = b;
            } else if (node.op == '[') {
                Box l = boxes[node.l], b(l.height / 2, l.height, l.width + 2);
                for (int j = 0; j < b.height; j++) {
                    b.lines[j][0] = '(';
                    b.lines[j][l.width + 1] = ')';
                }
                b.copy(l, 0, 1);
                boxes[i] = b;
            } else {
                Box l = boxes[node.l], r = boxes[node.r];
                char op = node.op;
                if (op == '/') {
                    Box b(l.height, l.height + 1 + r.height, max(l.width, r.width));

                    b.copy(l, 0, (b.width - l.width + 1) / 2);
                    for (int j = 0; j < b.width; j++) b.lines[l.height][j] = '-';
                    b.copy(r, l.height + 1, (b.width - r.width + 1) / 2);
                    boxes[i] = b;
                } else if (op == '^') {
                    Box b(r.height + l.lvc, l.height + r.height, l.width + r.width);

                    b.copy(l, r.height, 0);
                    b.copy(r, 0, l.width);
                    boxes[i] = b;
                } else {
                    int lvc = max(l.lvc, r.lvc);

                    Box b(lvc, lvc + 1 + max(l.height - 1 - l.lvc, r.height - 1 - r.lvc), l.width + 3 + r.width);
                    b.copy(l, lvc - l.lvc, 0);
                    b.copy(r, lvc - r.lvc, l.width + 3);
                    b.lines[lvc][l.width + 1] = op;
                    boxes[i] = b;
                }
            }
        }

        for (auto line : boxes[ast.root].lines) {
            line.erase(find_if(line.rbegin(), line.rend(), [](char c) { return !isspace(c); }).base(), line.end());
            cout << line << "\n";
        }
    }
}
