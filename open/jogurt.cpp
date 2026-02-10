#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto dfs = [&](auto &&self, int v = 1, int depth = 0) -> void {
        if (depth < n - 1) {
            cout << v << " ";
            self(self, v + (1 << (depth + 1)), depth + 1);
            self(self, v + (1 << depth), depth + 1);
            return;
        }

        cout << (v ^ ((1 << depth) - 1)) << " ";
    };
    dfs(dfs);
}
