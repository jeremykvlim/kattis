#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, c;
    cin >> p >> c;

    int turns = 0;
    vector<int> skips;
    auto dfs = [&](auto &&self, int count = 0) {
        if (count == c) {
            int pos = 1;
            for (int i = 0; i < skips.size(); i++) {
                pos = (pos + skips[i] + 1) % p;
                if (i < skips.size() - 1 && !pos) return;
            }
            turns |= 1 << pos;
            return;
        }

        for (int s = 1; s < p && count + s <= c; s++) {
            skips.emplace_back(s);
            self(self, count + s);
            skips.pop_back();
        }
    };
    dfs(dfs);

    if (turns == 1) cout << "yes\n";
    else if (!(turns & 1)) cout << "no\n";
    else cout << "maybe\n";
}
