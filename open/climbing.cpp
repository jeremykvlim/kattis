#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int _ = 0; _ < n; _++) {
        int __;
        cin >> __;

        while (__--) {
            int ___;
            cin >> ___;
        }
    }

    int t;
    cin >> t;

    auto generate = [&](auto &&self, int l, int r) -> vector<int> {
        if (l + 1 == r) return {r};

        int m = l + (r - l) / 2;
        auto sl = self(self, l, m), sr = self(self, m, r);

        vector<int> s;
        s.insert(s.end(), sl.begin(), sl.end());
        s.insert(s.end(), sr.begin(), sr.end());
        for (int i = sl.size() - 1; ~i; i--) s.emplace_back(sl[i]);
        return s;
    };


    vector<vector<bool>> pegs(t + 1, vector<bool>(n + 1, false));
    for (int j = 1; j <= t; j++) {
        int i;
        cin >> i;

        pegs[j] = pegs[j - 1];
        pegs[j][i] = !pegs[j][i];
    }

    vector<bool> state(t + 1, false);
    state[0] = true;
    vector<int> count(n + 1, 0), wet;
    for (int j : generate(generate, 0, t)) {
        state[j] = !state[j];

        if (state[j]) {
            for (int i = 1; i <= n; i++)
                if (pegs[j][i] && !count[i]++) wet.emplace_back(i);
        } else {
            for (int i = 1; i <= n; i++)
                if (pegs[j][i] && count[i]-- == 1) wet.emplace_back(i);
        }
    }

    cout << wet.size() << "\n";
    for (int i : wet) cout << i << "\n";
}
