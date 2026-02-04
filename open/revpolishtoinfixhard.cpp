#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    stack<int> s;
    vector<int> l, r;
    vector<string> ops, nums;
    string token;
    while (cin >> token)
        if (token.size() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')) {
            r.emplace_back(s.top());
            s.pop();
            l.emplace_back(s.top());
            s.pop();
            ops.emplace_back(token);
            s.emplace(ops.size() - 1);
            nums.emplace_back("");
        } else {
            l.emplace_back(-1);
            r.emplace_back(-1);
            ops.emplace_back("");
            s.emplace(ops.size() - 1);
            nums.emplace_back(token);
        }

    auto dfs = [&](auto &&self, int i) -> void {
        if (ops[i].empty()) {
            cout << nums[i];
            return;
        }
        cout << "(";
        self(self, l[i]);
        cout << ops[i];
        self(self, r[i]);
        cout << ")";
    };
    dfs(dfs, s.top());
}
