#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x;
    string s;
    auto input = [&]() {
        char c;
        for (;;) {
            if (!cin.get(c)) return false;
            if (c == '(') break;
        }
        if (!cin.get(c) || c == ')') return false;

        x = c - '0';
        for (;;) {
            if (!cin.get(c)) return false;
            if (c == ',') break;
            x = x * 10 + (c - '0');
        }

        s.clear();
        for (;;) {
            if (!cin.get(c)) return false;
            if (c == ')') break;
            s += c;
        }
        return true;
    };

    while (input()) {
        vector<int> tree(2, -1);
        auto add = [&]() {
            if (s.empty()) {
                tree[1] = x;
                return;
            }

            int i = 1;
            for (char c : s)
                if (c == 'L') i = i << 1;
                else i = i << 1 | 1;

            if (tree.size() < i + 1) tree.resize(i + 1, -1);
            tree[i] = x;
        };

        add();
        while (input()) add();

        vector<bool> visited(tree.size(), false);
        auto dfs = [&](auto &&self, int i = 1) -> void {
            if (i < tree.size() && tree[i] != -1) {
                visited[i] = true;
                self(self, i << 1);
                self(self, i << 1 | 1);
            }
        };
        dfs(dfs);

        for (int i = 1; i < tree.size(); i++)
            if (tree[i] != -1 && !visited[i]) goto next;

        for (int i = 1; i < tree.size(); i++)
            if (tree[i] != -1) cout << tree[i] << " ";
        cout << "\n";
        continue;

        next:;
        cout << "incomplete\n";
    }
}
