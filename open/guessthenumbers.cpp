#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        vector<int> values(n);
        for (int &v : values) cin >> v;
        sort(values.begin(), values.end());

        int m;
        string expression;
        cin >> m >> expression;

        unordered_map<char, int> unknowns;
        for (char &c : expression)
            if (isalpha(c)) c = unknowns.count(c) ? '0' + unknowns[c] : '0' + (unknowns[c] = unknowns.size());

        do {
            int i;
            auto dfs = [&](auto &&self, int j = 0) -> int {
                if (expression[j] != '(') {
                    i = j + 1;
                    return values[expression[j] - '0'];
                }
                int e1 = self(self, j + 1);
                char op = expression[i];
                int e2 = self(self, i + 1);
                i++;
                return op == '+' ? e1 + e2 : op == '-' ? e1 - e2 : e1 * e2;
            };

            if (dfs(dfs) == m) {
                cout << "YES\n";
                goto next;
            }
        } while (next_permutation(values.begin(), values.end()));
        cout << "NO\n";
        next:;
    }
}
