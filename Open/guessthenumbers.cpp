#include <bits/stdc++.h>
using namespace std;

int backtrack(int &i, int j, string expression, vector<int> values) {
    if (expression[j] != '(') {
        i = j + 1;
        return values[expression[j] - '0'];
    }
    int e1 = backtrack(i, j + 1, expression, values);
    char op = expression[i];
    int e2 = backtrack(i, i + 1, expression, values);
    i++;
    return op == '+' ? e1 + e2 : op == '-' ? e1 - e2 : e1 * e2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n && n) {
        int m;
        vector<int> values(n);
        string expression;

        for (int &v : values) cin >> v;
        cin >> m >> expression;

        unordered_map<char, int> unknowns;
        for (char &c : expression)
            if (isalpha(c)) c = unknowns.count(c) ? '0' + unknowns[c] : '0' + (unknowns[c] = unknowns.size());
        sort(values.begin(), values.end());

        do {
            int i;
            if (backtrack(i, 0, expression, values) == m) {
                cout << "YES\n";
                goto next;
            }
        } while (next_permutation(values.begin(), values.end()));
        cout << "NO\n";
        next:;
    }
}
