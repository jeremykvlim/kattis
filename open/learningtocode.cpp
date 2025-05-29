#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, string> vars;
    auto parse = [&](auto &&self, const string &s) -> string {
        if (s.empty()) return "";
        if (s[0] == '"') return s.substr(1, s.size() - 2);
        if (s[0] == '`') {
            string t;
            for (int i = 1; i + 1 < s.size(); i++) {
                if (s[i] == '{') {
                    t.pop_back();
                    int j = i + 1;
                    for (int count = 1; count; j++) {
                        count += s[j] == '{';
                        count -= s[j] == '}';
                    }

                    auto r = self(self, s.substr(i + 1, j - i - 2));
                    if (r == ".") return r;
                    t += r;
                    i = j - 1;
                } else t += s[i];

                if (t.size() > 1e4) return ".";
            }
            return t;
        }
        return vars[s];
    };

    string type;
    while (cin >> type && type != "end.")
        if (type == "var") {
            string name, _, rhs;
            cin >> name >> _;
            getline(cin, rhs);

            vars[name] = parse(parse, rhs.substr(1, rhs.size() - 2));
        } else {
            string expr;
            getline(cin, expr);

            cout << parse(parse, expr.substr(1, expr.size() - 2)) << "\n";
        }
}
