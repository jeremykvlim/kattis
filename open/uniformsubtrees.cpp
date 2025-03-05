#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int nodes = 0;
    vector<int> prev{-1};
    auto parse = [&](auto &&self, int i = 1) -> int {
        int curr = nodes;
        while (s[i] != ')') {
            prev.emplace_back(curr);
            nodes++;
            i = self(self, i + 1);
        }
        return i + 1;
    };
    parse(parse);

    vector<vector<int>> adj_list(nodes + 1), tree{{0}};
    for (int i = 1; i <= nodes; i++) {
        adj_list[prev[i]].emplace_back(i);
        vector<int> subtree{0};

        auto check = [&](auto &&self, int v, int k) -> bool {
            if (k < 0) return true;
            int count = 0;
            for (int u : adj_list[v])
                if (self(self, u, k - 1)) count++;
            return count >= subtree[k];
        };

        for (int j = i; j; j = prev[j]) {
            int children = 1;
            for (int u : adj_list[prev[j]])
                if (u != j) children += check(check, u, subtree.size() - 1);
            subtree.emplace_back(children);
        }
        reverse(subtree.begin(), subtree.end());
        tree.emplace_back(subtree);
    }

    sort(tree.begin(), tree.end());
    for (auto subtree : tree) {
        for (int children : subtree) cout << children << " ";
        cout << "\n";
    }
}
